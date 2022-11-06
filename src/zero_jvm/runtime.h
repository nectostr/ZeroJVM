#pragma once

#include <stdlib.h>
#include "constants.h"


typedef struct {
    char *name;
    unsigned char type;
    unsigned int offset;
} MapEntry; // static map entry

typedef struct {
    unsigned int max_statics_table_offset;
    char *statics_table;

    unsigned int max_statics_map_index;
    MapEntry *statics_map;
} Runtime;

extern Runtime runtime;


void init_runtime();

