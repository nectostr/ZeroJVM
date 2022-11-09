#include "frame.h"

Frame initialize_frame(JavaClass *current_class, unsigned char *bytecode) {
    Frame frame;
    frame.current_class = current_class;
    frame.bytecode = bytecode;

    unsigned short maxstack = (unsigned short) bytecode[0] << 8 | (unsigned short) bytecode[1];
    frame.stack = calloc(maxstack, sizeof(unsigned int));

    unsigned short maxlocals = (unsigned short) bytecode[2] << 8 | (unsigned short) bytecode[3];
    frame.locals = calloc(maxlocals, sizeof(unsigned int));

    frame.bytecode_length = (unsigned int) bytecode[4] << 24 | (unsigned int) bytecode[5] << 16 |
                            (unsigned int) bytecode[6] << 8  | (unsigned int) bytecode[7];
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
    unsigned char op;
    unsigned int stack_pointer = 0;
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