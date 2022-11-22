#include "frame.h"

uint32_t *descriptor2params(const char *descriptor, uint32_t *current_stack, uint16_t current_stack_pointer) {
    uint8_t flag = 1;  // 0 - stop, 1 - outside of object name, 2 - inside of object name
    uint32_t words = 0;
    while (flag) {
        if (flag == 1) {
            switch (*descriptor) {
                case 'L':
                    words++;
                    flag = 2;
                    break;
                case ';':
                    flag = 1;
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
            descriptor++;
        }
    }

    uint32_t *params = calloc(words + 1, sizeof(uint32_t));
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
    frame.stack = calloc(maxstack, sizeof(uint32_t));

    uint16_t maxlocals = (uint16_t) bytecode[2] << 8 | (uint16_t) bytecode[3];
    frame.locals = calloc(maxlocals, sizeof(uint32_t));
    memcpy(frame.locals, params, params_words * WORD_SIZE);

    frame.bytecode_length = (uint32_t) bytecode[4] << 24 | (uint32_t) bytecode[5] << 16 |
                            (uint32_t) bytecode[6] << 8 | (uint32_t) bytecode[7];
    frame.instruction_pointer = 8;
    return frame;
}

uint32_t * execute_frame(Frame *frame) {
    uint8_t op;
    uint16_t stack_pointer = 0;
    uint32_t * result_pointer = NULL;
    while (result_pointer == NULL) {
        op = frame->bytecode[frame->instruction_pointer];
        switch (op) {
            case 0xa7: {  // goto
                frame->instruction_pointer += 2;
                int16_t offset = frame->bytecode[frame->instruction_pointer - 1] << 8;
                offset |= frame->bytecode[frame->instruction_pointer];
                frame->instruction_pointer += offset;
                break;
            }
            case 0x02: {
                int val = -1;
                memcpy(&frame->stack[stack_pointer++], &val, sizeof(val));
                break;
            }
            case 0x03: {
                int val = 0;
                memcpy(&frame->stack[stack_pointer++], &val, sizeof(val));
                break;
            }
            case 0x04: {
                int val = 1;
                memcpy(&frame->stack[stack_pointer++], &val, sizeof(val));
                break;
            }
            case 0x05: {
                int val = 2;
                memcpy(&frame->stack[stack_pointer++], &val, sizeof(val));
                break;
            }
            case 0x06: {
                int val = 3;
                memcpy(&frame->stack[stack_pointer++], &val, sizeof(val));
                break;
            }
            case 0x07: {
                int val = 4;
                memcpy(&frame->stack[stack_pointer++], &val, sizeof(val));
                break;
            }
            case 0x08: {
                int val = 5;
                memcpy(&frame->stack[stack_pointer++], &val, sizeof(val));
                break;
            }
            case 0x60: {
                int val1 = 0;
                int val2 = 0;
                memcpy(&val1, &frame->stack[stack_pointer - 1], sizeof(val1));
                memcpy(&val2, &frame->stack[stack_pointer - 2], sizeof(val2));
                val1 = val1 + val2;
                memcpy(&frame->stack[stack_pointer - 2], &val1, sizeof(val1));
                stack_pointer--;
                break;
            }
            case 0x1a:
                memcpy(&frame->stack[stack_pointer++], &frame->locals[0], WORD_SIZE);
                break;
            case 0x1b:
                memcpy(&frame->stack[stack_pointer++], &frame->locals[1], WORD_SIZE);
                break;
            case 0x1c:
                memcpy(&frame->stack[stack_pointer++], &frame->locals[2], WORD_SIZE);
                break;
            case 0x1d:
                memcpy(&frame->stack[stack_pointer++], &frame->locals[3], WORD_SIZE);
                break;
            case 0x3b:
                memcpy(&frame->locals[0], &frame->stack[--stack_pointer], WORD_SIZE);
                break;
            case 0x3c:
                memcpy(&frame->locals[1], &frame->stack[--stack_pointer], WORD_SIZE);
                break;
            case 0x3d:
                memcpy(&frame->locals[2], &frame->stack[--stack_pointer], WORD_SIZE);
                break;
            case 0x3e:
                memcpy(&frame->locals[3], &frame->stack[--stack_pointer], WORD_SIZE);
                break;            
            case 0x4b: // astore_0
                memcpy(&frame->locals[0], &frame->stack[--stack_pointer], WORD_SIZE);
                break;
            case 0x4c: // astore_1
                memcpy(&frame->locals[1], &frame->stack[--stack_pointer], WORD_SIZE);
                break;
            case 0x4d: // astore_2
                memcpy(&frame->locals[2], &frame->stack[--stack_pointer], WORD_SIZE);
                break;
            case 0x4e: // astore_3
                memcpy(&frame->locals[3], &frame->stack[--stack_pointer], WORD_SIZE);
                break;
            case 0x3a: // astore
                memcpy(&frame->locals[frame->bytecode[frame->instruction_pointer + 1]], &frame->stack[--stack_pointer], WORD_SIZE);
                frame->instruction_pointer++;
                break;
            case 0x36: // istore
                frame->instruction_pointer++;
                memcpy(&frame->locals[frame->bytecode[frame->instruction_pointer]], &frame->stack[--stack_pointer], WORD_SIZE);
                break;
            case 0x15: // iload
                frame->instruction_pointer++;
                memcpy(&frame->stack[stack_pointer++], &frame->locals[frame->bytecode[frame->instruction_pointer]], WORD_SIZE);
                break;
            case 0x10: // bipush
            {
                int8_t val = frame->bytecode[++frame->instruction_pointer];
                memcpy(&frame->stack[stack_pointer++], &val, sizeof(val));
                break;
            }
            case 0x11: // sipush
            {
                int16_t val = ((uint16_t) frame->bytecode[frame->instruction_pointer + 1] << 8) |
                              frame->bytecode[frame->instruction_pointer + 2];
                memcpy(&frame->stack[stack_pointer++], &val, sizeof(val));
                frame->instruction_pointer += 2;
                break;
            }
            case 0x59: // dup
            {
                memcpy(&frame->stack[stack_pointer], &frame->stack[stack_pointer - 1], WORD_SIZE);
                stack_pointer++;
                break;
            }
            case 0xbb: // new
            {
                frame->instruction_pointer += 2;
                uint16_t index = ((uint16_t) frame->bytecode[frame->instruction_pointer - 1] << 8) |
                                 frame->bytecode[frame->instruction_pointer];
                ConstantPoolEntry *data = &frame->current_class->constant_pool[index];
                short value[2];
                memcpy(&value, &data->data, 4);

//                uint16_t name_and_type_index = value[0];
                uint16_t class_index = value[1];

                char *method_name = calloc(1, 3);
                strcpy(method_name, "CL");

                class_index = frame->current_class->constant_pool[class_index].data.ushort;
                char *classname = frame->current_class->constant_pool[class_index].addon;

                unsigned long fullmethodnamelen = strlen(classname) + strlen(method_name) + 2;
                char *fullname = calloc(fullmethodnamelen, sizeof(char));
                strcpy(fullname, method_name);
                fullname[strlen(method_name)] = '.';
                strcpy(&fullname[strlen(method_name) + 1], classname);

                uint8_t ** statics = find_static_method(fullname, 0, MAP_TYPE_CL);
                JavaClass * class = (JavaClass *) *statics;

                uint32_t * template = (uint32_t *) calloc(1, sizeof(*(class->object_instance_template)));
                memcpy(template, class->object_instance_template, sizeof(*(class->object_instance_template)));
                
                memcpy(&frame->stack[stack_pointer++], &template, sizeof(template));
                break;
            }
            case 0xb6:  // invokevirtual
            {
                frame->instruction_pointer += 2;
                // uint16_t index = ((uint16_t) frame->bytecode[frame->instruction_pointer - 1] << 8) |
                //                  frame->bytecode[frame->instruction_pointer];
                // ConstantPoolEntry *data = &frame->current_class->constant_pool[index];
                // short value[2];
                // memcpy(&value, &data->data, 4);
                break;
            }
            case 0xb7:  // invokespecial
            {
                frame->instruction_pointer += 2;
                uint16_t index = ((uint16_t) frame->bytecode[frame->instruction_pointer - 1] << 8) |
                                 frame->bytecode[frame->instruction_pointer];
                ConstantPoolEntry *data = &frame->current_class->constant_pool[index];
                short value[2];
                memcpy(&value, &data->data, 4);

                // uint16_t name_and_type_index = value[0];
                // uint16_t class_index = value[1];

                // memcpy(&value, &frame->current_class->constant_pool[name_and_type_index].data.uint, 4);
                // char *descriptor = frame->current_class->constant_pool[value[0]].addon;
                // char *method_name = frame->current_class->constant_pool[value[1]].addon;

                // class_index = frame->current_class->constant_pool[class_index].data.ushort;
                // char *classname = frame->current_class->constant_pool[class_index].addon;

                // unsigned long fullmethodnamelen = strlen(classname) + strlen(method_name) + 2;
                // char *fullname = calloc(fullmethodnamelen, sizeof(char));
                // strcpy(fullname, classname);
                // fullname[strlen(classname)] = '.';
                // strcpy(&fullname[strlen(classname) + 1], method_name);

                // uint8_t **new_method_code = find_static_method(fullname, descriptor, 0, MAP_TYPE_SM);

                // uint32_t *params = descriptor2params(descriptor, frame->stack, stack_pointer);

                // // TODO: fix current class to proper class of callee
                // Frame new_frame = initialize_frame(frame->current_class, *new_method_code, params[0], &params[1]);
                // uint32_t * result = execute_frame(&new_frame);
                // //execute_frame(&new_frame);
                // // Put the return value on the stack from frame
                // // TODO: check if result should be one or two words
                // memcpy(&frame->stack[stack_pointer++], result, WORD_SIZE);
                // free(result);
                break;
            }
            case 0xb8:  // invokestatic
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

                unsigned long fullmethodnamelen = strlen(classname) + strlen(method_name) + 2;
                char *fullname = calloc(fullmethodnamelen, sizeof(char));
                strcpy(fullname, classname);
                fullname[strlen(classname)] = '.';
                strcpy(&fullname[strlen(classname) + 1], method_name);

                uint8_t **new_method_code = find_static_method(fullname, descriptor, MAP_TYPE_SM);

                uint32_t *params = descriptor2params(descriptor, frame->stack, stack_pointer);

                // TODO: fix current class to proper class of callee
                Frame new_frame = initialize_frame(frame->current_class, *new_method_code, params[0], &params[1]);
                uint32_t * result = execute_frame(&new_frame);
                //execute_frame(&new_frame);
                // Put the return value on the stack from frame
                // TODO: check if result should be one or two words
                memcpy(&frame->stack[stack_pointer++], result, WORD_SIZE);
                free(result);
                break;
            }
            case 0xba:  // invokedynamic
            {
                break;
            }
            case 0xac: // ireturn
            {
                // TODO: make sure stack clean?   
                result_pointer = calloc(1, WORD_SIZE);
                memcpy(result_pointer, &frame->stack[--stack_pointer], WORD_SIZE);
                break;
            }
            case 0xb1: // return
            {
                result_pointer = calloc(1, WORD_SIZE);
                break;
            }
            case 0xad: // lreturn 
            {
                result_pointer = calloc(1, WORD_SIZE);
                memcpy(result_pointer, &frame->stack[--stack_pointer], WORD_SIZE);
                memcpy(result_pointer+WORD_SIZE, &frame->stack[--stack_pointer], WORD_SIZE);
                break;                
            }
           


            default:
                printf("ERROR: not implemented operation: %d !\n", op);
                break;
        }
        frame->instruction_pointer++;
    }

    if (frame->instruction_pointer - 8 != frame->bytecode_length) {
        printf(
                "Frame pointer != frame bytecode length in the end of frame execution: %u != %u\n",
                frame->instruction_pointer,
                frame->bytecode_length
        );
    }
    free(frame->stack);
    free(frame->locals);

    return result_pointer;
}