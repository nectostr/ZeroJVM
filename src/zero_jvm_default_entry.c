#include <stdint.h>
#include "zero_jvm/loader.h"
#include "zero_jvm/runtime.h"

#define FILENAME "entrypoint.class"

int32_t main(int argc, char** argv) {
    init_runtime();
    JavaClass entrypoint = read_class(FILENAME);
    debug_print_statics_table();
    debug_print_statics_map();

    debug_print_map(&entrypoint);
    debug_print_rep(&entrypoint);
    return 0;
}