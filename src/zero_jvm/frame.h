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

Frame initialize_frame(JavaClass *current_class, uint8_t *bytecode, uint16_t params_words, uint32_t * params);

void finalize_frame(Frame *frame);

uint32_t * execute_frame(Frame *frame);