#include <stdint.h>
#include "zero_jvm/loader.h"
#include "zero_jvm/runtime.h"
#include "zero_jvm/frame.h"

#define FILENAME "Entrypoint.class"

int32_t main(int argc, char **argv) {
    init_runtime();
    JavaClass *entrypoint = read_class(FILENAME);

//
//    debug_print_statics_table();
//    debug_print_statics_map();
//
//    debug_print_map(entrypoint);
//    debug_print_rep(entrypoint);
//    debug_print_obj_tmpl(entrypoint);

    // find public static void main(String[] args) and run
    uint8_t **main = (uint8_t **) (runtime.statics_table +
                                   find_static_record("Entrypoint.main", "([Ljava/lang/String;)V", MAP_TYPE_SM));
    Frame frame = initialize_frame(entrypoint, *main, 0, NULL);
    uint32_t *result = execute_frame(&frame);
    free(result);
    printf("ENDOFMAIN SUCCESS\n");
    return 0;
}