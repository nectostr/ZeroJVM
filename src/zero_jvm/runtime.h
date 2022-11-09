#pragma once

#include <stdlib.h>
#include <stdint.h>
#include "constants.h"

typedef struct {
    uint16_t attribute_name_index;
    uint32_t attribute_length;
    uint8_t *info;
} AttributeInfo;

typedef struct {
    char *name;
    uint8_t type;
    uint32_t offset;
    uint16_t access_flags;
    uint16_t attributes_count;
    AttributeInfo *attributes;    
    uint16_t descriptor_index;
    char *descriptor;
} MapEntry; // static map entry

typedef struct {
    uint32_t max_statics_table_offset;
    uint8_t *statics_table;

    uint32_t max_statics_map_index;
    MapEntry *statics_map;
} Runtime;

extern Runtime runtime;


void init_runtime();

