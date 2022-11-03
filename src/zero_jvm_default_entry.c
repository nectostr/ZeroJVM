#include <stdint.h>
#include "zero_jvm/structures.h"
#include "zero_jvm/loader.h"


int32_t main(int argc, char** argv) {
    //return read_class();
    statics_map[0].name[0] = *"a";
    statics_map[0].type = 1;
    statics_map[0].offset = 0;

    statics_table[0] = 1;
    debug_print_statics_table();
    debug_print_statics_map();
    return 0;
}