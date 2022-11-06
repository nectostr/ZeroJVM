#pragma once
#ifndef LOADER_H
#define LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "entrypoint.class"

#define ACC_PUBLIC 	    0x0001 	// Marked or implicitly public in source.
#define ACC_PROTECTED 	0x0004 	// Marked protected in source.
#define ACC_PRIVATE 	0x0002  //	Marked private in source.
#define ACC_STATIC 	    0x0008 	// Marked or implicitly static in source.
#define ACC_FINAL   	0x0010 	// Marked or implicitly final in source.
#define ACC_INTERFACE 	0x0200 	// Was an interface in source.
#define ACC_ABSTRACT 	0x0400 	// Marked or implicitly abstract in source.
#define ACC_SYNTHETIC 	0x1000 	// Declared synthetic; not present in the source code.
#define ACC_ANNOTATION 	0x2000 	// Declared as an annotation type.
#define ACC_ENUM 	    0x4000 	// Declared as an enum type. 

extern unsigned char filebytebuffer[8];
extern FILE *filepointer;
extern struct ConstantPoolEntry *constant_table;

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

typedef struct{
    unsigned short access_flags;
    unsigned short name_index;
    unsigned short descriptor_index;
    unsigned short attributes_count;
    struct AttributeInfo *attributes;
} MFInfo;

void loadfile();

unsigned char read_uint8();
unsigned short read_uint16();
unsigned int read_uint32();
unsigned long read_uint64();

struct ConstantPoolEntry read_constant_pool_entry(unsigned char tag);
struct MethodInfo read_method_info();
struct FieldInfo read_field_info();
struct AttributeInfo read_attribute_info();
char * get_constant_pool_entry_name(int index);

int read_class();


#include "structures.h"

#endif // LOADER_H