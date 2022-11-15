#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "constants.h"
#include "runtime.h"


union ConstantPoolType {
    uint16_t ushort;
    uint32_t uint;
    uint64_t ulong;
};

typedef struct {
    uint8_t tag;
    union ConstantPoolType data;
    char *addon;
} ConstantPoolEntry;

typedef struct {
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attributes_count;
    AttributeInfo *attributes;
} MFInfo;


typedef struct {
    uint16_t constant_pool_size;
    ConstantPoolEntry *constant_pool;
    uint16_t this;  // look in the constant pool
    uint16_t super;  // look in the constant pool
    uint16_t access_flags;
    uint16_t field_count;
    uint16_t method_count;
    uint16_t attribute_count;
    AttributeInfo *attributes;
    

    // They also are in statics table, btw
    MapEntry *class_map;
    uint32_t max_class_map_index;;
    uint8_t *class_rep;
    uint32_t max_class_rep_offset;
    uint32_t max_class_field_offset;

    uint8_t * object_instance_template;
} JavaClass;

extern uint8_t filebytebuffer[8];
extern FILE *filepointer;

void loadfile(const char *classname);

static uint8_t read_uint8();

static uint16_t read_uint16();

static uint32_t read_uint32();

__attribute__((unused)) static uint64_t read_uint64();

ConstantPoolEntry read_constant_pool_entry(uint8_t tag);

MFInfo read_meth_field_info();

AttributeInfo read_attribute_info();

char *get_constant_pool_entry_name(JavaClass *class, uint32_t index);

void add_statics_entry(JavaClass *class, MFInfo *info);

uint8_t ** find_static_method(char *name, char *signature, uint16_t access_flags, uint8_t type);

void add_instance_entry(JavaClass *class, MFInfo *info);

JavaClass * read_class(char *classname);

void debug_print_statics_map();

void debug_print_statics_table();

void debug_print_rep(JavaClass *class);

void debug_print_map(JavaClass *class);

void debug_print_obj_tmpl(JavaClass *class);