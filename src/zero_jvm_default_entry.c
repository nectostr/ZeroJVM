#include <stdint.h>
#include "zero_jvm/structures.h"
#include "zero_jvm/loader.h"


int32_t main(int argc, char** argv) {
    read_class();
    // debug_random_init();
    debug_print_statics_table();
    debug_print_statics_map();
    return 0;
}