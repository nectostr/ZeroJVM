#include <stdint.h>
#include "zero_jvm/loader.h"
#include "zero_jvm/runtime.h"
#include "zero_jvm/frame.h"

#define FILENAME "Simple.class"

int32_t main(int argc, char** argv) {
    init_runtime();
    JavaClass entrypoint = read_class(FILENAME);

    debug_print_statics_table();
    debug_print_statics_map();

    debug_print_map(&entrypoint);
    debug_print_rep(&entrypoint);
    debug_print_obj_tmpl(&entrypoint);

    // find public static void main(String[] args) and run
    uint8_t ** main = find_static_method("Simple.main", "([Ljava/lang/String;)V", 0x0001 | 0x0008);
    Frame frame = initialize_frame(&entrypoint, *main, 0, NULL);
    execute_frame(&frame);
    return 0;
}