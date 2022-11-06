#include <stdint.h>
#include "zero_jvm/loader.h"

#define FILENAME "entrypoint.class"

int32_t main(int argc, char** argv) {
    JavaClass entrypoint = read_class(FILENAME);
    debug_print_statics_table(&entrypoint);
    debug_print_statics_map(&entrypoint);
    return 0;
}