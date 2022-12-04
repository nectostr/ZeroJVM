#include "frame.h"

uint32_t *
descriptor2params(const char *descriptor, uint32_t *current_stack, uint16_t current_stack_pointer, uint8_t objectref) {
    if (objectref != 0) {
        objectref = 1;
    }
    uint8_t flag = 1;  // 0 - stop, 1 - outside of object name, 2 - inside of object name
    uint32_t words = objectref;
    while (flag) {
        if (flag == 1) {
            switch (*descriptor) {
                case 'L':
                    words++;
                    flag = 2;
                    break;
                case ')':
                    flag = 0;
                    break;
                case 'Z':
                case 'B':
                case 'S':
                case 'F':
                case 'C':
                case 'I':
                    words++;
                    break;
                case 'J':
                case 'D':
                    words += 2;
                    break;
                default:
                    break;
            }
        } else if (flag == 2) {
            if (*descriptor == ';') {
                flag = 1;
            }
        }
        descriptor++;
    }

    uint32_t *params = custom_calloc(words + 1, sizeof(uint32_t));
    params[0] = words;
    for (uint32_t i = 0; i < words; i++) {
        memcpy(&params[words - i], &current_stack[current_stack_pointer - i - 1], WORD_SIZE);
    }

    return params;
}

Frame initialize_frame(JavaClass *current_class, uint8_t *bytecode, uint16_t params_words, uint32_t *params) {
    Frame frame;
    frame.current_class = current_class;
    frame.bytecode = bytecode;

    uint16_t maxstack = (uint16_t) bytecode[0] << 8 | (uint16_t) bytecode[1];
    frame.stack = custom_calloc(maxstack, sizeof(uint32_t));

    uint16_t maxlocals = (uint16_t) bytecode[2] << 8 | (uint16_t) bytecode[3];
    if (maxlocals > 0) {
        frame.locals = custom_calloc(maxlocals, sizeof(uint32_t));
        memcpy(frame.locals, params, params_words * WORD_SIZE);
    } else {
        frame.locals = NULL;
    }

    frame.bytecode_length = (uint32_t) bytecode[4] << 24 | (uint32_t) bytecode[5] << 16 |
                            (uint32_t) bytecode[6] << 8 | (uint32_t) bytecode[7];
    frame.instruction_pointer = 8;
    return frame;
}

uint32_t *execute_frame(Frame *frame) {
    uint8_t op;
    uint16_t stack_pointer = 0;
    uint32_t *result_pointer = NULL;
    while (result_pointer == NULL) {
        op = frame->bytecode[frame->instruction_pointer];
        switch (op) {
            case 0x00:  // nop
                break;
            case 0x01:  // aconst_null
                frame->stack[stack_pointer++] = 0;
                break;
            case 0x02:
            case 0x03:
            case 0x04:
            case 0x05:
            case 0x06:
            case 0x07:
            case 0x08: // iconst_x
            {
                int val = op - 0x03;
                memcpy(&frame->stack[stack_pointer++], &val, WORD_SIZE);
                break;
            }
            case 0x0b:  // fconst_0
            case 0x0c:  // fconst_1
            case 0x0d:  // fconst_2
                frame->stack[stack_pointer] = 0;
                frame->stack[stack_pointer + 1] = op - 0x0b;
                stack_pointer += 2;
                break;
            case 0x12: // ldc
            {
                uint8_t index = frame->bytecode[++frame->instruction_pointer];
                ConstantPoolEntry *data = &frame->current_class->constant_pool[index];
                switch (data->tag) {
                    case 3:
                    case 4:
                        memcpy(&frame->stack[stack_pointer++], &data->data.uint, WORD_SIZE);
                        break;
                    case 8: {
                        uint32_t val = data->data.ushort;
                        memcpy(&frame->stack[stack_pointer++], &val, WORD_SIZE);
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case 0xa5:  // if_acmpeq
            case 0xa6:  // if_acmpne
            case 0x9f:  // if_icmpeq
            case 0xa0:  // if_icmpne
            case 0xa1:  // if_icmplt
            case 0xa2:  // if_icmpge
            case 0xa3:  // if_icmpgt
            case 0xa4:  // if_icmple
            {
                uint16_t offset = (uint16_t) frame->bytecode[frame->instruction_pointer + 1] << 8 |
                                  (uint16_t) frame->bytecode[frame->instruction_pointer + 2];
                frame->instruction_pointer += 2;
                int32_t val1 = 0;
                int32_t val2 = 0;
                memcpy(&val1, &frame->stack[stack_pointer - 2], WORD_SIZE);
                memcpy(&val2, &frame->stack[stack_pointer - 1], WORD_SIZE);
                stack_pointer -= 2;
                uint8_t condition = 0;
                switch (op) {
                    case 0xa5:
                        condition = val1 == val2;
                        break;
                    case 0xa6:
                        condition = val1 != val2;
                        break;
                    case 0x9f:
                        condition = val1 == val2;
                        break;
                    case 0xa0:
                        condition = val1 != val2;
                        break;
                    case 0xa1:
                        condition = val1 < val2;
                        break;
                    case 0xa2:
                        condition = val1 >= val2;
                        break;
                    case 0xa3:
                        condition = val1 > val2;
                        break;
                    case 0xa4:
                        condition = val1 <= val2;
                        break;
                    default:
                        break;
                }
                if (condition) {
                    frame->instruction_pointer += offset - 2;  // -2 because from opcode location (ignoring branchbytes)
                    frame->instruction_pointer--;
                }
                break;
            }
            case 0x99:  // ifeq
            case 0x9a:  // ifne
            case 0x9b:  // iflt
            case 0x9c:  // ifge
            case 0x9d:  // ifgt
            case 0x9e:  // ifle
            case 0xc7:  // ifnonnull
            case 0xc6:  // ifnull
            {
                uint16_t offset = (uint16_t) frame->bytecode[frame->instruction_pointer + 1] << 8 |
                                  (uint16_t) frame->bytecode[frame->instruction_pointer + 2];
                frame->instruction_pointer += 2;

                int32_t val1 = 0;
                memcpy(&val1, &frame->stack[--stack_pointer], WORD_SIZE);
                uint8_t condition = 0;
                switch (op) {
                    case 0x99:
                        condition = val1 == 0;
                        break;
                    case 0x9a:
                        condition = val1 != 0;
                        break;
                    case 0x9b:
                        condition = val1 < 0;
                        break;
                    case 0x9c:
                        condition = val1 >= 0;
                        break;
                    case 0x9d:
                        condition = val1 > 0;
                        break;
                    case 0x9e:
                        condition = val1 <= 0;
                        break;
                    case 0xc7:
                        condition = val1 != 0;
                        break;
                    case 0xc6:
                        condition = val1 == 0;
                        break;
                    default:
                        break;
                }
                if (condition) {
                    frame->instruction_pointer += offset - 2; // -2 because from opcode location (ignoring branchbytes)
                    frame->instruction_pointer--;
                }
                break;
            }
            case 0xa7: // goto
            {
                frame->instruction_pointer += 2;
                int16_t offset = frame->bytecode[frame->instruction_pointer - 1] << 8 |
                                 frame->bytecode[frame->instruction_pointer];
                frame->instruction_pointer += offset - 2; // -2 because from opcode location (ignoring branchbytes)
                frame->instruction_pointer--;
                break;
            }
            case 0x60: // iadd
            case 0x64: // isub
            case 0x68: // imul
            case 0x6c: // idiv
            case 0x70: // irem
            case 0x78: // ishl
            case 0x7a: // ishr
            {
                int val1 = 0;
                int val2 = 0;
                memcpy(&val1, &frame->stack[stack_pointer - 1], WORD_SIZE);
                memcpy(&val2, &frame->stack[stack_pointer - 2], WORD_SIZE);
                switch (op) {
                    case 0x60:
                        val1 = val1 + val2;
                        break;
                    case 0x64:
                        val1 = val2 - val1;
                        break;
                    case 0x68:
                        val1 = val1 * val2;
                        break;
                    case 0x6c:
                        val1 = val2 / val1;
                        break;
                    case 0x70:
                        val1 = val2 % val1;
                        break;
                    case 0x78:
                        val1 = val2 << val1;
                        break;
                    case 0x7a:
                        val1 = val2 >> val1;
                        break;
                    default:
                        break;
                }
                memcpy(&frame->stack[stack_pointer - 2], &val1, WORD_SIZE);
                stack_pointer--;
                break;
            }
            case 0x62: //fadd
            case 0x66: //fsub
            case 0x6a: //fmul
            case 0x6e: //fdiv
            {
                float val1 = 0;
                float val2 = 0;
                memcpy(&val1, &frame->stack[stack_pointer - 1], WORD_SIZE);
                memcpy(&val2, &frame->stack[stack_pointer - 2], WORD_SIZE);
                switch (op) {
                    case 0x62:
                        val1 = val1 + val2;
                        break;
                    case 0x66:
                        val1 = val2 - val1;
                        break;
                    case 0x6a:
                        val1 = val1 * val2;
                        break;
                    case 0x6e:
                        val1 = val2 / val1;
                        break;
                    default:
                        break;
                }
                memcpy(&frame->stack[stack_pointer - 2], &val1, WORD_SIZE);
                stack_pointer--;
                break;
            }
            case 0x2a: // aload_0
            case 0x2b: // aload_1
            case 0x2c: // aload_2
            case 0x2d: // aload_3
                memcpy(&frame->stack[stack_pointer++], &frame->locals[op - 0x2a], WORD_SIZE);
                break;
            case 0x1a: // iload_0
            case 0x1b: // iload_1
            case 0x1c: // iload_2
            case 0x1d: // iload_3
                memcpy(&frame->stack[stack_pointer++], &frame->locals[op - 0x1a], WORD_SIZE);
                break;
            case 0x22: // fload_0
            case 0x23: // fload_1
            case 0x24: // fload_2
            case 0x25: // fload_3
                memcpy(&frame->stack[stack_pointer++], &frame->locals[op - 0x22], WORD_SIZE);
                break;
            case 0x3b: // istore_0
            case 0x3c: // istore_1
            case 0x3d: // istore_2
            case 0x3e: // istore_3
                memcpy(&frame->locals[op - 0x3b], &frame->stack[--stack_pointer], WORD_SIZE);
                break;
            case 0x4b: // astore_0
            case 0x4c: // astore_1
            case 0x4d: // astore_2
            case 0x4e: // astore_3
                memcpy(&frame->locals[op - 0x4b], &frame->stack[--stack_pointer], WORD_SIZE);
                break;
            case 0x43: // fstore_0
            case 0x44: // fstore_1
            case 0x45: // fstore_2
            case 0x46: // fstore_3
                memcpy(&frame->locals[op - 0x43], &frame->stack[--stack_pointer], WORD_SIZE);
                break;
            case 0x3a: // astore
            case 0x36: // istore
            case 0x38: // fstore
                frame->instruction_pointer++;
                memcpy(&frame->locals[frame->bytecode[frame->instruction_pointer]], &frame->stack[--stack_pointer],
                       WORD_SIZE);
                break;
            case 0x19: // aload
            case 0x15: // iload
            case 0x17: // fload
                frame->instruction_pointer++;
                memcpy(&frame->stack[stack_pointer++], &frame->locals[frame->bytecode[frame->instruction_pointer]],
                       WORD_SIZE);
                break;
            case 0x10: // bipush
            {
                int32_t val = *(int8_t *) &frame->bytecode[++frame->instruction_pointer];
                memcpy(&frame->stack[stack_pointer], &val, WORD_SIZE);
                stack_pointer++;
                break;
            }
            case 0x11: // sipush
            {
                uint32_t preval = frame->bytecode[frame->instruction_pointer + 1] << 8 |
                              frame->bytecode[frame->instruction_pointer + 2];
                int32_t val = *(int16_t *) &preval;
                memcpy(&frame->stack[stack_pointer++], &val, WORD_SIZE);
                frame->instruction_pointer += 2;
                break;
            }
            case 0x57: // pop
                stack_pointer--;
                break;
            case 0x59: // dup
                memcpy(&frame->stack[stack_pointer], &frame->stack[stack_pointer - 1], WORD_SIZE);
                stack_pointer++;
                break;
            case 0x86: // i2f
            {
                int32_t val = 0;
                memcpy(&val, &frame->stack[stack_pointer - 1], WORD_SIZE);
                float fval = (float) val;
                memcpy(&frame->stack[stack_pointer - 1], &fval, WORD_SIZE);
                break;
            }
            case 0x8b: // f2i
            {
                float val = 0;
                memcpy(&val, &frame->stack[stack_pointer - 1], WORD_SIZE);
                int32_t ival = (int32_t) val;
                memcpy(&frame->stack[stack_pointer - 1], &ival, WORD_SIZE);
                break;
            }
            case 0xbb: // new
            {
                frame->instruction_pointer += 2;
                uint16_t index = ((uint16_t) frame->bytecode[frame->instruction_pointer - 1] << 8) |
                                 frame->bytecode[frame->instruction_pointer];
                ConstantPoolEntry *data = &frame->current_class->constant_pool[index];
                uint16_t class_index = data->data.ushort;

//                class_index = frame->current_class->constant_pool[class_index].data.ushort;
                char *classname = frame->current_class->constant_pool[class_index].addon;

                char *fullname = combine_names_with_dot("CL", classname);
                JavaClass *class = *((JavaClass **) (runtime.statics_table +
                                                     find_static_record(fullname, 0, MAP_TYPE_CL)));
                free(fullname);

                uint32_t *template = (uint32_t *) custom_calloc(2 + class->field_count * 2, WORD_SIZE);
                memcpy(template, class->object_instance_template, class->field_count * 2 * WORD_SIZE);

                memcpy(&frame->stack[stack_pointer++], &template, WORD_SIZE);
                break;
            }
            case 0xb6:  // invokevirtual
            case 0xb7:  // invokespecial
            case 0xb8:  // invokestatic
            case 0xba:  // invokedynamic
            {
                frame->instruction_pointer += 2;
                uint16_t index = ((uint16_t) frame->bytecode[frame->instruction_pointer - 1] << 8) |
                                 frame->bytecode[frame->instruction_pointer];
                ConstantPoolEntry *data = &frame->current_class->constant_pool[index];
                short value[2];
                memcpy(&value, &data->data, 4);

                uint16_t name_and_type_index = value[0];
                uint16_t class_index = value[1];

                memcpy(&value, &frame->current_class->constant_pool[name_and_type_index].data.uint, 4);
                char *descriptor = frame->current_class->constant_pool[value[0]].addon;
                char *method_name = frame->current_class->constant_pool[value[1]].addon;

                class_index = frame->current_class->constant_pool[class_index].data.ushort;
                char *classname = frame->current_class->constant_pool[class_index].addon;

                char *fullname = combine_names_with_dot(classname, method_name);

                uint8_t type = MAP_TYPE_SM;
                uint8_t objectref = 0;
                if (op == 0xb7) {
                    type = MAP_TYPE_SIM;
                    objectref = 1;
                }
                if (op == 0xba || op == 0xb6) {
                    type = MAP_TYPE_IM;
                    objectref = 1;
                }

                uint32_t *params = descriptor2params(descriptor, frame->stack, stack_pointer, objectref);
                stack_pointer -= params[0]; // how many arguments from the stack we should throw away

                if (strcmp(fullname, "java/lang/Object.<init>") == 0) {
                    free(params);
                    free(fullname);
                    break;
                }
                if (strcmp(fullname, "Entrypoint.beep") == 0) {
                    // custom beep function
#ifndef X86
                    float freq = *(float *) &params[1];
                    int duration = *(int *) &params[2];
                    furi_check(furi_mutex_acquire(instance->model_mutex, FuriWaitForever) == FuriStatusOk);
                    FURI_LOG_I("ZERO_JVM", "Frequency %d.%.6d", (int)freq, (int)((freq-(int)freq)*1000000));
                    furi_hal_speaker_start(freq, 1.0f);
                    furi_delay_ms(duration);
                    furi_hal_speaker_stop();
                    FURI_LOG_I("ZERO_JVM", "Frequency %d.%.6d finished", (int)freq, (int)((freq-(int)freq)*1000000));
                    furi_mutex_release(instance->model_mutex);
                    FURI_LOG_I("ZERO_JVM", "mutex released");
#endif
                    free(params);
                    break;
                }
                if (strcmp(fullname, "Entrypoint.delay") == 0) {

#ifndef X86
                    furi_delay_ms(params[1]);
#endif
                    free(params);
                    break;
                }

                char *target_class_name = combine_names_with_dot("CL", classname);
                JavaClass *class = *((JavaClass **) (runtime.statics_table +
                                                     find_static_record(target_class_name, 0, MAP_TYPE_CL)));
                free(target_class_name);

                if (strcmp(fullname, "Entrypoint.println") == 0) {
                    free(fullname);
                    printf("Entering println\n");
                    const uint16_t capacity = 20;
                    if (strcmp(descriptor, "(I)V") == 0) {
                        char buffer[capacity];
#ifdef X86
                        snprintf(buffer, capacity, "%d", params[1]);
#endif
#ifndef X86
                        snprintf(buffer, capacity, "%ld", params[1]);
#endif
                        println(buffer);
                    } else if (strcmp(descriptor, "(Ljava/lang/String;)V") == 0) {
                        char *string = class->constant_pool[params[1]].addon;
                        println(string);
                    } else if (strcmp(descriptor, "(F)V") == 0) {
                        char buffer[capacity];
                        float val = *(float *) &params[1];
                        snprintf(buffer, capacity, "%d.%.6d", (int)val,   (int)((val-(int)val)*1000000));
                        println(buffer);
                    } else {
                        printf("Unknown descriptor %s", descriptor);
                    }
                    free(params);
                    break;
                }

                uint8_t **new_method_code = NULL;
                if (op == 0xba || op == 0xb6) {
                    new_method_code =
                            (uint8_t **) (class->class_rep + find_instance_offset(class, fullname, descriptor, type));
                } else {
                    new_method_code = (uint8_t **) (runtime.statics_table +
                                                    find_static_record(fullname, descriptor, type));
                }

                Frame new_frame = initialize_frame(class, *new_method_code, params[0], &params[1]);
                uint32_t *result = execute_frame(&new_frame);

                // Put the return value on the stack from frame
                // TODO: double
                if (strcmp(&descriptor[strlen(descriptor) - 2], ")V") != 0) {
                    memcpy(&frame->stack[stack_pointer++], result, WORD_SIZE);
                }
                free(result);
                free(fullname);
                free(params);
                break;
            }
            case 0xac: // ireturn
                // TODO: make sure stack clean?   
                result_pointer = custom_calloc(1, WORD_SIZE);
                memcpy(result_pointer, &frame->stack[--stack_pointer], WORD_SIZE);
                break;
            case 0xb1: // return
                result_pointer = custom_calloc(1, WORD_SIZE);
                break;
            case 0xad: // lreturn
                result_pointer = custom_calloc(1, WORD_SIZE);
                memcpy(result_pointer, &frame->stack[--stack_pointer], WORD_SIZE);
                memcpy(result_pointer + WORD_SIZE, &frame->stack[--stack_pointer], WORD_SIZE);
                break;
            case 0xb2: // getstatic
            case 0xb4: // getfield
            {
                frame->instruction_pointer += 2;
                uint16_t index = ((uint16_t) frame->bytecode[frame->instruction_pointer - 1] << 8) |
                                 frame->bytecode[frame->instruction_pointer];
                ConstantPoolEntry *data = &frame->current_class->constant_pool[index];
                short value[2];
                memcpy(&value, &data->data, 4);

                uint16_t name_and_type_index = value[0];
                uint16_t class_index = value[1];

                memcpy(&value, &frame->current_class->constant_pool[name_and_type_index].data.uint, 4);
                char *descriptor = frame->current_class->constant_pool[value[0]].addon;
                char *field_name = frame->current_class->constant_pool[value[1]].addon;

                class_index = frame->current_class->constant_pool[class_index].data.ushort;
                char *classname = frame->current_class->constant_pool[class_index].addon;

                char *fullname;
                uint8_t *address = NULL;
                if (op == 0xb4) {
                    fullname = combine_names_with_dot("CL", classname);
                    JavaClass *class = *((JavaClass **) (runtime.statics_table +
                                                         find_static_record(fullname, 0, MAP_TYPE_CL)));

                    address = (uint8_t *) frame->stack[--stack_pointer];
                    uint32_t offset = find_instance_offset(class, field_name, descriptor, MAP_TYPE_IF);
                    address += offset;
                } else {
                    fullname = combine_names_with_dot(classname, field_name);
                    address = (uint8_t *) (runtime.statics_table +
                                           find_static_record(fullname, descriptor, MAP_TYPE_SF));
                }

                memcpy(&frame->stack[stack_pointer++], address, WORD_SIZE);
                free(fullname);
                break;
            }
            case 0xb3: // putstatic
            case 0xb5: // putfield
            {
                frame->instruction_pointer += 2;
                uint16_t index = ((uint16_t) frame->bytecode[frame->instruction_pointer - 1] << 8) |
                                 frame->bytecode[frame->instruction_pointer];
                ConstantPoolEntry *data = &frame->current_class->constant_pool[index];

                short value[2];
                memcpy(&value, &data->data, 4);

                uint16_t name_and_type_index = value[0];
                uint16_t class_index = value[1];

                memcpy(&value, &frame->current_class->constant_pool[name_and_type_index].data.uint, 4);
                char *descriptor = frame->current_class->constant_pool[value[0]].addon;
                char *field_name = frame->current_class->constant_pool[value[1]].addon;

                class_index = frame->current_class->constant_pool[class_index].data.ushort;
                char *classname = frame->current_class->constant_pool[class_index].addon;

                uint32_t value_to_put = frame->stack[--stack_pointer];

                uint8_t *address = NULL;
                char *fullname = NULL;
                if (op == 0xb5) {
                    fullname = combine_names_with_dot("CL", classname);
                    JavaClass *class = *((JavaClass **) (runtime.statics_table +
                                                         find_static_record(fullname, 0, MAP_TYPE_CL)));
                    free(fullname);

                    address = (uint8_t *) frame->stack[--stack_pointer];
                    uint32_t offset = find_instance_offset(class, field_name, descriptor, MAP_TYPE_IF);
                    address += offset;
                } else {
                    fullname = combine_names_with_dot(classname, field_name);
                    address = (uint8_t *) (runtime.statics_table +
                                           find_static_record(fullname, descriptor, MAP_TYPE_SF));
                    free(fullname);
                }
                memcpy(address, &value_to_put, WORD_SIZE); // not working with double
                break;
            }
            case 0x84: // iinc
            {
                frame->instruction_pointer += 2;
                uint8_t index = frame->bytecode[frame->instruction_pointer - 1];
                int8_t value = 0;
                memcpy(&value, &frame->bytecode[frame->instruction_pointer], 1);
                frame->locals[index] += value;
                break;
            }

            default:
#ifdef X86
                printf("ERROR: not implemented operation: %d !\n", op);
#endif
                break;
        }
        frame->instruction_pointer++;
    }

    if (frame->instruction_pointer - 8 != frame->bytecode_length) {
        printf("Frame pointer != frame bytecode length in the end of frame execution\n");
    }
    free(frame->stack);
    if (frame->locals != NULL) {
        free(frame->locals);
    }
    return result_pointer;
}