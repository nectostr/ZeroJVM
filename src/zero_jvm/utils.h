#pragma once

#include <stdint.h>
#include <stdio.h>


#ifdef X86
void FURI_LOG_I(const char* tag, const char* format, ...);
#endif
#ifndef X86
#include <storage/storage.h>
extern File* entrypointfile;
#endif


void *custom_calloc (size_t nmemb, size_t size);
FILE *custom_fopen (const char *__restrict filename, const char *__restrict modes);
int custom_fclose (FILE *stream);
size_t custom_fread (void *__restrict ptr, size_t size, size_t n, FILE *__restrict stream);


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

    uint8_t *object_instance_template;
} JavaClass;

char *combine_names_with_dot(char *firstname, char *lastname);

