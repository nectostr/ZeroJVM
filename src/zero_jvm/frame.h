#pragma once

#include "loader.h"

typedef struct {
    JavaClass *current_class;
    unsigned int instruction_pointer;
    unsigned int bytecode_length;
    unsigned char *bytecode;
    unsigned int *locals;
    unsigned int *stack;
} Frame;

Frame initialize_frame(JavaClass *current_class, unsigned char *bytecode);

void finalize_frame(Frame *frame);

void execute_frame(Frame *frame);