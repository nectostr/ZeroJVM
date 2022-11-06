
#include "loader.h"

unsigned char filebytebuffer[8];
FILE *filepointer;

void loadfile(const char *classname) {
    filepointer = fopen(classname, "rb");
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

ConstantPoolEntry read_constant_pool_entry(unsigned char tag) {
    ConstantPoolEntry entry;
    entry.tag = tag;
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

MFInfo read_meth_field_info() {
    MFInfo info;
    info.access_flags = read_uint16();
    info.name_index = read_uint16();
    info.descriptor_index = read_uint16();
    info.attributes_count = read_uint16();
    info.attributes = calloc(info.attributes_count, sizeof(AttributeInfo));
    for (unsigned short i = 0; i < info.attributes_count; i++) {
        info.attributes[i] = read_attribute_info();
    }
    return info;
}

AttributeInfo read_attribute_info() {
    AttributeInfo attribute;
    attribute.attribute_name_index = read_uint16();
    attribute.attribute_length = read_uint32();
    attribute.info = calloc(attribute.attribute_length, 1);
    for (unsigned int i = 0; i < attribute.attribute_length; i++) {
        attribute.info[i] = read_uint8();
    }
    return attribute;
}

char *get_constant_pool_entry_name(JavaClass * class, int index) {
    ConstantPoolEntry *entry = &class->constant_pool[index];
    while (entry->tag != 1) {
        entry = &class->constant_pool[entry->data.ushort];
    }
    return (char *) entry->addon;

}


/*
[Z = boolean
[B = byte
[S = short
[I = int
[J = long
[F = float
[D = double
[C = char
[L = any non-primitives(Object)
*/

void add_statics_entry(JavaClass * class, MFInfo *info) {
    /*
        * 1. Copy name as a string to statics_map
        * 2. Find out type and chage offset accordingly
        * 3. Convert types to inner types
        * 4. Update class->max_statics_map_index

    */

    class->statics_map[class->max_statics_map_index].offset = class->max_statics_table_offset;
    char *entry = class->statics_table + class->max_statics_table_offset;
    int tag = info->name_index;
    class->statics_map[class->max_statics_map_index].name = get_constant_pool_entry_name(class, tag);

    tag = info->descriptor_index;
    char *type = get_constant_pool_entry_name(class, tag);
    if ((strcmp(type, "J") == 0) ||
        (strcmp(type, "D") == 0)) {  //if long/double
        class->max_statics_table_offset += 8;
    } else {
        class->max_statics_table_offset += 4;
    }

    switch (*type) { //type
        case 'Z':
        case 'B':
        case 'S':
        case 'F':
        case 'C':
        case 'I': //int, float
        {
            unsigned int *tmp4 = (unsigned int *) entry;
            *tmp4 = 0;
            class->statics_map[class->max_statics_map_index].type = SF;
            break;
        }
        case 'J':
        case 'D': //long double
        {
            unsigned long *tmp8 = (unsigned long *) entry;
            *tmp8 = 0;
            class->statics_map[class->max_statics_map_index].type = SF;
            break;
        }
        case '(': //method,
            *entry = 'C';// TODO: default compiler/interpreter link
            if (strcmp(class->statics_map[class->max_statics_map_index].name, "<init>") == 0) {
                class->statics_map[class->max_statics_map_index].type = SIM;
            } else {
                class->statics_map[class->max_statics_map_index].type = SM;
            }
            break;
            //case ?: //Map, Ref, SIM
    }
    class->max_statics_map_index++;
}

JavaClass read_class(char *classname) {
    JavaClass class;
    class.max_statics_map_index = 0;
    class.statics_map = calloc(STATICS_MAP_SIZE, sizeof(MapEntry));

    class.max_statics_table_offset = 0;
    class.statics_table = calloc(STATICS_TABLE_SIZE, sizeof(char));

    loadfile(classname);
    read_uint32(); // cafebabe
    read_uint16(); // minor version
    read_uint16(); // major version

    class.constant_pool_size = read_uint16();
    class.constant_pool = calloc(class.constant_pool_size, sizeof(ConstantPoolEntry));
    for (unsigned short i = 1; i < class.constant_pool_size; i++) {
        unsigned char tag = read_uint8();
        class.constant_pool[i] = read_constant_pool_entry(tag);
        if (tag == 5 || tag == 6) {
            i++;  // 8-byte constants like double or long
        }
    }

    class.access_flags = read_uint16();
    class.this = read_uint16();
    class.super = read_uint16();
    unsigned short interface_count = read_uint16(); // should be always 0
    if (interface_count > 0) {
        // panic
        exit(1);
    }

    class.field_count = read_uint16();
    for (unsigned short i = 0; i < class.field_count; ++i) {
        MFInfo current_field = read_meth_field_info();
        // TODO: if static, add to statics table
        if (current_field.access_flags & ACC_STATIC) {
            add_statics_entry(&class, &current_field);
        }
        // TODO: implement instance method logic
    }

    class.method_count = read_uint16();
    for (unsigned short i = 0; i < class.method_count; ++i) {
        MFInfo current_method = read_meth_field_info();
        char *name = get_constant_pool_entry_name(&class, current_method.name_index);

        if ((strcmp(name, "<clinit>") != 0) &&
            ((current_method.access_flags & ACC_STATIC) ||
             (strcmp(name, "<init>") == 0))) {
            add_statics_entry(&class, &current_method);
        }
        // TODO: implement instance method logic
    }

    class.attribute_count = read_uint16();
    class.attributes = calloc(class.attribute_count, sizeof(AttributeInfo));
    for (unsigned short i = 0; i < class.attribute_count; ++i) {
        class.attributes[i] = read_attribute_info();
    }

    long bytes_read = ftell(filepointer);
    fseek(filepointer, 0L, SEEK_END);
    long bytes_total = ftell(filepointer);

    printf("Bytes read: %ld, bytes total: %ld \n", bytes_read, bytes_total);
    fclose(filepointer);
    return class;
}


// DEBUG FUNCTIONS
void debug_print_statics_table(JavaClass * class) {
    printf("STATICS_TABLE\n");
    for (int i = 0; i < class->max_statics_table_offset; i = i + 4) {
        printf("%d\n", class->statics_table[i]);
    }
    printf("\n");
}

void debug_print_statics_map(JavaClass * class) {
    printf("STATICS_MAP\n");
    for (int i = 0; i < class->max_statics_map_index; i++) {
        printf("%s %d %d\n", class->statics_map[i].name, class->statics_map[i].type, class->statics_map[i].offset);
    }
    printf("\n");
}