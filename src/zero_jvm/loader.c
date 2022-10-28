
#include <stdio.h>
#include <stdlib.h>

#define FILENAME "entrypoint.class"

unsigned char filebytebuffer[8];
FILE *filepointer;

void loadfile() {
    filepointer = fopen(FILENAME, "rb");
}

unsigned char read_uint8() {
    fread(filebytebuffer, 1, 1, filepointer);
    return filebytebuffer[0];
}

unsigned short read_uint16() {
    fread(filebytebuffer, 2, 1, filepointer);
    return (unsigned short) filebytebuffer[0] << 8 | (unsigned short) filebytebuffer[1];
}

unsigned int read_uint32() {
    fread(filebytebuffer, 4, 1, filepointer);
    return (unsigned int) filebytebuffer[0] << 24 | (unsigned int) filebytebuffer[1] << 16 |
           (unsigned int) filebytebuffer[2] << 8 | (unsigned int) filebytebuffer[3];
}

unsigned long read_uint64() {
    fread(filebytebuffer, 8, 1, filepointer);
    return (unsigned long) filebytebuffer[0] << 56 | (unsigned long) filebytebuffer[1] << 48 |
           (unsigned long) filebytebuffer[2] << 40 | (unsigned long) filebytebuffer[3] << 32 |
           (unsigned long) filebytebuffer[4] << 24 | (unsigned long) filebytebuffer[5] << 16 |
           (unsigned long) filebytebuffer[6] << 8 | (unsigned long) filebytebuffer[7];
}

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

struct ConstantPoolEntry read_constant_pool_entry() {
    struct ConstantPoolEntry entry;
    entry.tag = read_uint8();
    switch (entry.tag) {
        case 1:
            // utf8
            entry.data.ushort = read_uint16();
            entry.addon = calloc(entry.data.ushort, 1);
            for (unsigned short i = 0; i < entry.data.ushort; i++) {
                entry.addon[i] = read_uint8();
            }
            break;
        case 3:
            // integer
        case 4:
            // float
            entry.data.uint = read_uint32();
            break;
        case 5:
            // long
        case 6:
            // double
            entry.data.ulong = ((long) read_uint32() << 32) + read_uint32();
            break;
        case 7:
            // class
        case 8:
            // string
            entry.data.ushort = read_uint16();
            break;
        case 9:
            // fieldref
        case 10:
            // methodref
        case 12:
            // name and type
            entry.data.uint = ((int) read_uint16()) << 16;  // read first 2 bytes
            entry.data.uint |= read_uint16();  // read second 2 bytes
            break;
        case 16:
            // method type
        case 19:
            // module
            entry.data.ushort = read_uint16();
            break;
    }
    return entry;
}

struct AttributeInfo {
    unsigned short attribute_name_index;
    unsigned int attribute_length;
    unsigned char *info;
};

struct AttributeInfo read_attribute_info() {
    struct AttributeInfo attribute;
    attribute.attribute_name_index = read_uint16();
    attribute.attribute_length = read_uint32();
    attribute.info = calloc(attribute.attribute_length, 1);
    for (unsigned int i = 0; i < attribute.attribute_length; i++) {
        attribute.info[i] = read_uint8();
    }
    return attribute;
}


struct FieldInfo {
    unsigned short access_flags;
    unsigned short name_index;
    unsigned short descriptor_index;
    unsigned short attributes_count;
    struct AttributeInfo *attributes;
};


struct FieldInfo read_field_info() {
    struct FieldInfo info;
    info.access_flags = read_uint16();
    info.name_index = read_uint16();
    info.descriptor_index = read_uint16();
    info.attributes_count = read_uint16();
    info.attributes = calloc(info.attributes_count, sizeof(struct AttributeInfo));
    for (unsigned short i = 0; i < info.attributes_count; i++) {
        info.attributes[i] = read_attribute_info();
    }
    return info;
}

struct MethodInfo {
    unsigned short access_flags;
    unsigned short name_index;
    unsigned short descriptor_index;
    unsigned short attributes_count;
    struct AttributeInfo *attributes;
};


struct MethodInfo read_method_info() {
    struct MethodInfo info;
    info.access_flags = read_uint16();
    info.name_index = read_uint16();
    info.descriptor_index = read_uint16();
    info.attributes_count = read_uint16();
    info.attributes = calloc(info.attributes_count, sizeof(struct AttributeInfo));
    for (unsigned short i = 0; i < info.attributes_count; i++) {
        info.attributes[i] = read_attribute_info();
    }
    return info;
}


int read_class() {
    loadfile();
    unsigned int cafebabe = read_uint32();
    unsigned short minor = read_uint16();
    unsigned short major = read_uint16();
    unsigned short constant_pool_count = read_uint16();

    struct ConstantPoolEntry *constant_table = calloc(constant_pool_count, sizeof(struct ConstantPoolEntry));

    for (unsigned short i = 1; i < constant_pool_count; i++) {
        constant_table[i] = read_constant_pool_entry();
    }
    unsigned short access_flags = read_uint16();
    unsigned short this_class = read_uint16();
    unsigned short super_class = read_uint16();
    unsigned short interfaces_count = read_uint16(); // should be always 0
    if (interfaces_count > 0) {
        // panic
        exit(1);
    }

    unsigned short fields_count = read_uint16();
    struct FieldInfo *field_table = calloc(fields_count, sizeof(struct FieldInfo));

    for (unsigned short i = 0; i < fields_count; ++i) {
        field_table[i] = read_field_info();
    }

    unsigned short methods_count = read_uint16();
    struct MethodInfo *method_table = calloc(fields_count, sizeof(struct MethodInfo));
    for (unsigned short i = 0; i < methods_count; ++i) {
        method_table[i] = read_method_info();
    }

    unsigned short attributes_count = read_uint16();
    struct AttributeInfo *attribute_table = calloc(fields_count, sizeof(struct AttributeInfo));
    for (unsigned short i = 0; i < attributes_count; ++i) {
        attribute_table[i] = read_attribute_info();
    }

    long bytes_read = ftell(filepointer);
    fseek(filepointer, 0L, SEEK_END);
    long bytes_total = ftell(filepointer);

    printf("Bytes read: %ld, bytes total: %ld ", bytes_read, bytes_total);
    return 0;
}