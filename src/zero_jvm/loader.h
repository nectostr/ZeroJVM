#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "runtime.h"

union ConstantPoolType {
    unsigned short ushort;
    unsigned int uint;
    unsigned long ulong;
};

typedef struct {
    unsigned char tag;
    union ConstantPoolType data;
    unsigned char *addon;
} ConstantPoolEntry;

typedef struct {
    unsigned short attribute_name_index;
    unsigned int attribute_length;
    unsigned char *info;
} AttributeInfo;

typedef struct {
    unsigned short access_flags;
    unsigned short name_index;
    unsigned short descriptor_index;
    unsigned short attributes_count;
    AttributeInfo *attributes;
} MFInfo;


typedef struct {
    unsigned short constant_pool_size;
    ConstantPoolEntry *constant_pool;
    unsigned short this;  // look in the constant pool
    unsigned short super;  // look in the constant pool
    unsigned short access_flags;
    unsigned short field_count;
    unsigned short method_count;
    unsigned short attribute_count;
    AttributeInfo *attributes;

    MapEntry * class_map;
    unsigned int max_class_map_index;; 
    char * class_rep;
    unsigned int max_class_rep_offset;
    unsigned int max_class_field_offset;
} JavaClass;

extern unsigned char filebytebuffer[8];
extern FILE *filepointer;
extern ConstantPoolEntry *constant_table;

void loadfile(const char *classname);

unsigned char read_uint8();

unsigned short read_uint16();

unsigned int read_uint32();

unsigned long read_uint64();

ConstantPoolEntry read_constant_pool_entry(unsigned char tag);

MFInfo read_meth_field_info();

AttributeInfo read_attribute_info();

char *get_constant_pool_entry_name(JavaClass *class, int index);

void add_statics_entry(char * name, char * type);

void add_instance_entry(JavaClass *class, MFInfo *info);

JavaClass read_class(char *classname);

void debug_print_statics_map();

void debug_print_statics_table();

void debug_print_rep(JavaClass *class);

void debug_print_map(JavaClass *class);