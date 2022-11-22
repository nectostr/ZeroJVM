#pragma once

#include <stdlib.h>
#include <stdint.h>
#include "constants.h"
#include "utils.h"


typedef struct {
    uint32_t max_statics_table_offset;
    uint8_t *statics_table;

    uint32_t max_statics_map_index;
    MapEntry *statics_map;
} Runtime;

extern Runtime runtime;


void init_runtime();

