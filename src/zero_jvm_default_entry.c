#include <stdint.h>
#include "zero_jvm/structures.h"
#include "zero_jvm/loader.h"

void assign_string(char* str, char* value, int size) {
    for (int i = 0; i < size; i++) {
        str[i] = value[i];
    }
}

int32_t main(int argc, char** argv) {
    //return read_class();
    assign_string(statics_map[0].name, "a", 1);
    assign_string(statics_map[0].type, "b", 1);
    statics_map[0].offset = 0;

    statics_table[0] = 1;
    debug_print_statics_table();
    return 0;
}