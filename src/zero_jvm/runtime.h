#pragma once

#include <stdlib.h>
#include "constants.h"

typedef struct {
    unsigned short attribute_name_index;
    unsigned int attribute_length;
    unsigned char *info;
} AttributeInfo;

typedef struct {
    char *name;
    unsigned char type;
    unsigned int offset;
    unsigned short access_flags;
    unsigned short attributes_count;
    AttributeInfo *attributes;    
    unsigned short descriptor_index;
} MapEntry; // static map entry

typedef struct {
    unsigned int max_statics_table_offset;
    char *statics_table;

    unsigned int max_statics_map_index;
    MapEntry *statics_map;
} Runtime;

extern Runtime runtime;


void init_runtime();

