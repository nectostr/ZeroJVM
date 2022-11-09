#include "frame.h"

Frame initialize_frame(JavaClass *current_class, uint8_t *bytecode) {
    Frame frame;
    frame.current_class = current_class;
    frame.bytecode = bytecode;

    uint16_t maxstack = (uint16_t) bytecode[0] << 8 | (uint16_t) bytecode[1];
    frame.stack = calloc(maxstack, sizeof(uint32_t));

    uint16_t maxlocals = (uint16_t) bytecode[2] << 8 | (uint16_t) bytecode[3];
    frame.locals = calloc(maxlocals, sizeof(uint32_t));

    frame.bytecode_length = (uint32_t) bytecode[4] << 24 | (uint32_t) bytecode[5] << 16 |
                            (uint32_t) bytecode[6] << 8 | (uint32_t) bytecode[7];
    frame.instruction_pointer = 8;

    // TODO: copy params to locals from somewhere?
    return frame;
}

void finalize_frame(Frame *frame) {
    if (frame->instruction_pointer != frame->bytecode_length) {
        printf(
                "Frame pointer != frame bytecode length in the end of frame execution: %ui != %ui",
                frame->instruction_pointer,
                frame->bytecode_length
        );
    }
    free(frame->stack);
    free(frame->locals);
}

void execute_frame(Frame *frame) {
    uint8_t op;
    uint16_t stack_pointer = 0;
    while (1) {
        op = frame->bytecode[frame->instruction_pointer];
        switch (op) {
            case 0x00:
                break;
            case 0x01:
                break;
            default:
                break;
        }
        frame->instruction_pointer++;
    }
}