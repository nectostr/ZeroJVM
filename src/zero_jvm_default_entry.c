#include <stdint.h>
#include "zero_jvm/loader.h"
#include "zero_jvm/runtime.h"

#define FILENAME "entrypoint.class"

int32_t main(int argc, char** argv) {
    init_runtime();
    JavaClass entrypoint = read_class(FILENAME);
    debug_print_statics_table();
    debug_print_statics_map();
    return 0;
}