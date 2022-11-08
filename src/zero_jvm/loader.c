
#include "loader.h"

unsigned char filebytebuffer[8];
FILE *filepointer;

void loadfile(const char *classname) {
    filepointer = fopen(classname, "rb");
}

static unsigned char read_uint8() {
    fread(filebytebuffer, 1, 1, filepointer);
    return filebytebuffer[0];
}

static unsigned short read_uint16() {
    fread(filebytebuffer, 2, 1, filepointer);
    return (unsigned short) filebytebuffer[0] << 8 | (unsigned short) filebytebuffer[1];
}

static unsigned int read_uint32() {
    fread(filebytebuffer, 4, 1, filepointer);
    return (unsigned int) filebytebuffer[0] << 24 | (unsigned int) filebytebuffer[1] << 16 |
           (unsigned int) filebytebuffer[2] << 8 | (unsigned int) filebytebuffer[3];
}

__attribute__((unused)) static unsigned long read_uint64() {
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

char *get_constant_pool_entry_name(JavaClass *class, int index) {
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

void add_statics_entry(JavaClass *class, MFInfo *info) {
    /*
        * 1. Copy name as a string to statics_map
        * 2. Find out type and chage offset accordingly
        * 3. Convert types to inner types
        * 4. Update runtime.max_statics_map_index

    */

    char *name = get_constant_pool_entry_name(class, info->name_index);
    char *type = get_constant_pool_entry_name(class, info->descriptor_index);

    runtime.statics_map[runtime.max_statics_map_index].offset = runtime.max_statics_table_offset;
    char *entry = runtime.statics_table + runtime.max_statics_table_offset;
    runtime.statics_map[runtime.max_statics_map_index].name = name;
    runtime.statics_map[runtime.max_statics_map_index].access_flags = info->access_flags;
    runtime.statics_map[runtime.max_statics_map_index].attributes_count = info->attributes_count;
    runtime.statics_map[runtime.max_statics_map_index].attributes = info->attributes;

    if ((strcmp(type, "J") == 0) ||
        (strcmp(type, "D") == 0)) {  //if long/double
        runtime.max_statics_table_offset += 8;
    } else {
        runtime.max_statics_table_offset += 4;
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
            runtime.statics_map[runtime.max_statics_map_index].type = MAP_TYPE_SF;
            break;
        }
        case 'J':
        case 'D': //long double
        {
            unsigned long *tmp8 = (unsigned long *) entry;
            *tmp8 = 0;
            runtime.statics_map[runtime.max_statics_map_index].type = MAP_TYPE_SF;
            break;
        }
        case '(': //method,
            *entry = 'C';// TODO: default compiler/interpreter link
            if (strcmp(name, "<init>") == 0) {
                runtime.statics_map[runtime.max_statics_map_index].type = MAP_TYPE_SIM;
            } else {
                runtime.statics_map[runtime.max_statics_map_index].type = MAP_TYPE_SM;
            }
            break;
            //case ?: //Map, Ref, SIM
    }
    runtime.max_statics_map_index++;
}

/// Go to static table and find method by full name (Class.name) and signature
void find_static_method(char *name, char *signature, unsigned short access_flags) {
    for (unsigned int i = 0; i < runtime.max_statics_map_index; i++) {
        if (
                runtime.statics_map[i].type == MAP_TYPE_SM
                && (runtime.statics_map[i].access_flags & access_flags) == access_flags
                && strcmp(runtime.statics_map[i].name, name) == 0
                ) {
            // TODO: check method signature
            unsigned int offset = runtime.statics_map[i].offset;

        }
    }
}


void add_instance_entry(JavaClass *class, MFInfo *info) {

    char *name = get_constant_pool_entry_name(class, info->name_index);
    char *type = get_constant_pool_entry_name(class, info->descriptor_index);
    char *entry = class->class_rep + class->max_class_rep_offset;
    char *object_entry = class->object_instance_template + class->max_class_field_offset;

    class->class_map[class->max_class_map_index].name = name;
    class->class_map[class->max_class_map_index].access_flags = info->access_flags;
    class->class_map[class->max_class_map_index].attributes_count = info->attributes_count;
    class->class_map[class->max_class_map_index].attributes = info->attributes;
    class->class_map[class->max_class_map_index].descriptor_index = info->descriptor_index;


    switch (*type) { //type
        case 'Z':
        case 'B':
        case 'S':
        case 'F':
        case 'C':
        case 'I': //int, float
        {
            unsigned int *tmp4 = (unsigned int *) object_entry;
            *tmp4 = 0;

            class->class_map[class->max_class_map_index].offset = class->max_class_field_offset;
            class->max_class_field_offset += 4;

            class->class_map[class->max_class_map_index].type = MAP_TYPE_SF;
            break;
        }
        case 'J':
        case 'D': //long double
        {
            unsigned long *tmp8 = (unsigned long *) object_entry;
            *tmp8 = 0;

            class->class_map[class->max_class_map_index].offset = class->max_class_field_offset;
            class->max_class_field_offset += 8;

            class->class_map[class->max_class_map_index].type = MAP_TYPE_SF;
            break;
        }
        case '(': //method,
            class->class_map[class->max_class_map_index].offset = class->max_class_rep_offset;
            class->max_class_rep_offset += 4;
            *entry = 'C';// TODO: default compiler/interpreter link
            if (strcmp(name, "<init>") == 0) {
                class->class_map[class->max_class_map_index].type = MAP_TYPE_SIM;
            } else {
                class->class_map[class->max_class_map_index].type = MAP_TYPE_SM;
            }
            break;
            //case ?: //Map, Ref, SIM
    }
    class->max_class_map_index++;
}

JavaClass read_class(char *classname) {
    JavaClass class;


    class.max_class_field_offset = WORD_SIZE * 2; //  because 2 words taken for info
    class.max_class_rep_offset = 0;
    class.max_class_map_index = 0;


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

    class.class_map = calloc(MAX_THEORETICAL_CLASS_MAP_SIZE, sizeof(MapEntry));

    class.object_instance_template = calloc(class.field_count * 2, WORD_SIZE); // see any other * 2 explanation

    for (unsigned short i = 0; i < class.field_count; ++i) {
        MFInfo current_field = read_meth_field_info();
        char *name = get_constant_pool_entry_name(&class, current_field.name_index);
        char *type = get_constant_pool_entry_name(&class, current_field.descriptor_index);
        if (current_field.access_flags & ACC_STATIC) {
            add_statics_entry(&class, &current_field);
        } else {
            // Technically, we should set the class rep before first this, but this will never use rep, so it's ok
            add_instance_entry(&class, &current_field);
        }
    }

    class.method_count = read_uint16();

    class.class_rep = calloc(class.method_count, WORD_SIZE);
    // Because we copy class red address, that callocs only here. 
    unsigned int *header = (unsigned int *) class.object_instance_template;
    memcpy(header, &class.class_rep, WORD_SIZE);
    header++;
    unsigned int tmp = 100;
    memcpy(header, &tmp, WORD_SIZE);
    //*header = the rest of header;

    for (unsigned short i = 0; i < class.method_count; ++i) {
        MFInfo current_method = read_meth_field_info();
        char *name = get_constant_pool_entry_name(&class, current_method.name_index);
        char *type = get_constant_pool_entry_name(&class, current_method.descriptor_index);
        if (strcmp(name, "<clinit>") == 0) {
            //TODO: "Just call me, I am always last"
        } else if ((current_method.access_flags & ACC_STATIC) ||
                   (strcmp(name, "<init>") == 0)) {
            add_statics_entry(&class, &current_method);
        } else {
            add_instance_entry(&class, &current_method);
        }
    }

    // add map and rep to statics
    runtime.statics_map[runtime.max_statics_map_index].name = "Rep";
    runtime.statics_map[runtime.max_statics_map_index].access_flags = 0;
    runtime.statics_map[runtime.max_statics_map_index].attributes_count = 0;
    runtime.statics_map[runtime.max_statics_map_index].attributes = NULL;
    runtime.statics_map[runtime.max_statics_map_index].offset = runtime.max_statics_table_offset;
    memcpy(runtime.statics_table + runtime.max_statics_table_offset, &class.class_rep, WORD_SIZE);
    runtime.max_statics_table_offset += 4;
    runtime.statics_map[runtime.max_statics_map_index].type = MAP_TYPE_REP;
    runtime.max_statics_map_index++;


    runtime.statics_map[runtime.max_statics_map_index].name = "Map";
    runtime.statics_map[runtime.max_statics_map_index].access_flags = 0;
    runtime.statics_map[runtime.max_statics_map_index].attributes_count = 0;
    runtime.statics_map[runtime.max_statics_map_index].attributes = NULL;
    runtime.statics_map[runtime.max_statics_map_index].offset = runtime.max_statics_table_offset;
    memcpy(runtime.statics_table + runtime.max_statics_table_offset, &class.class_map, WORD_SIZE);
    runtime.max_statics_table_offset += 4;
    runtime.statics_map[runtime.max_statics_map_index].type = MAP_TYPE_MAP;
    runtime.max_statics_map_index++;

    MapEntry *temp_map = calloc(class.field_count * 2 + class.method_count,
                                sizeof(MapEntry)); // * 2 because we have no idea if they would be long/double or normal guys
    // (though we know, but too inefficient to count them)
    memcpy(temp_map, class.class_map, class.max_class_map_index * sizeof(MapEntry));
    free(class.class_map);
    class.class_map = temp_map;

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
void debug_print_statics_table() {
    printf("STATICS_TABLE\n");
    unsigned int *ptr = malloc(4);
    for (int i = 0; i < runtime.max_statics_table_offset; i = i + 4) {
        memcpy(ptr, &(runtime.statics_table[i]), 4);
//        printf("%p\t", *ptr);
        printf("%d\n", *ptr);
    }
    free(ptr);
    printf("\n");
}

void debug_print_statics_map() {
    printf("STATICS_MAP\n");
    for (int i = 0; i < runtime.max_statics_map_index; i++) {
        printf("%s %d %d\n", runtime.statics_map[i].name, runtime.statics_map[i].type, runtime.statics_map[i].offset);
    }
    printf("\n");
}

// DEBUG FUNCTIONS
void debug_print_rep(JavaClass *class) {
    printf("%s REP\n", get_constant_pool_entry_name(class, class->this));
    unsigned int *ptr = malloc(4);
    for (int i = 0; i < class->max_class_rep_offset; i = i + 4) {
        memcpy(ptr, &class->class_rep[i], 4);
//        printf("%p\n", *ptr);
        printf("%d\n", *ptr);
    }
    free(ptr);
    printf("\n");
}

void debug_print_map(JavaClass *class) {
    printf("%s MAP\n", get_constant_pool_entry_name(class, class->this));
    for (int i = 0; i < class->max_class_map_index; i++) {
        printf("%s %d %d\n", class->class_map[i].name, class->class_map[i].type, class->class_map[i].offset);
    }
    printf("\n");
}


// DEBUG FUNCTIONS
void debug_print_obj_tmpl(JavaClass *class) {
    printf("%s obj template\n", get_constant_pool_entry_name(class, class->this));
    unsigned int *ptr = malloc(4);
    memcpy(ptr, &class->object_instance_template[0], 4);
//    printf("%p\n", *ptr);
    printf("%d\n", *ptr);
    for (int i = 1; i < class->max_class_field_offset; i = i + 4) {
        memcpy(ptr, &class->object_instance_template[i], 4);
        printf("%d\n", *ptr);
    }
    free(ptr);
    printf("\n");
}