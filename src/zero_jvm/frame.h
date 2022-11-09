#pragma once

#include "loader.h"

typedef struct {
    JavaClass *current_class;
    uint32_t instruction_pointer;
    uint32_t bytecode_length;
    uint8_t *bytecode;
    uint32_t *locals;
    uint32_t *stack;
} Frame;

Frame initialize_frame(JavaClass *current_class, uint8_t *bytecode);

void finalize_frame(Frame *frame);

void execute_frame(Frame *frame);