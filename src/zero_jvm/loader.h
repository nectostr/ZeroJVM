#pragma once
#ifndef LOADER_H
#define LOADER_H

#include <stdio.h>
#include <stdlib.h>

#define FILENAME "entrypoint.class"

extern unsigned char filebytebuffer[8];
extern FILE *filepointer;

union ConstantPoolType {
    unsigned short ushort;
    unsigned int uint;
    unsigned long ulong;
};

struct ConstantPoolEntry {
    unsigned char tag;
    union ConstantPoolType data;
    unsigned char *addon;
};

struct AttributeInfo {
    unsigned short attribute_name_index;
    unsigned int attribute_length;
    unsigned char *info;
};

struct FieldInfo {
    unsigned short access_flags;
    unsigned short name_index;
    unsigned short descriptor_index;
    unsigned short attributes_count;
    struct AttributeInfo *attributes;
};

struct MethodInfo {
    unsigned short access_flags;
    unsigned short name_index;
    unsigned short descriptor_index;
    unsigned short attributes_count;
    struct AttributeInfo *attributes;
};

void loadfile();

unsigned char read_uint8();
unsigned short read_uint16();
unsigned int read_uint32();
unsigned long read_uint64();

struct ConstantPoolEntry read_constant_pool_entry(unsigned char tag);
struct MethodInfo read_method_info();
struct FieldInfo read_field_info();
struct AttributeInfo read_attribute_info();

int read_class();

#endif // LOADER_H