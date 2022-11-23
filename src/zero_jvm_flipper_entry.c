#define FILENAME "Simple.class"

#include <furi.h>
#include <gui/gui.h>
#include <stdint.h>
#include "zero_jvm/loader.h"
#include "zero_jvm/runtime.h"
#include "zero_jvm/frame.h"



int32_t zero_jvm_entrypoint(void* p) {
    UNUSED(p);
    init_runtime();
    JavaClass *entrypoint = read_class(FILENAME);

    // find public static void main(String[] args) and run
    uint8_t **main = (uint8_t **) (runtime.statics_table +
                                   find_static_record("Simple.main", "([Ljava/lang/String;)V", MAP_TYPE_SM));
    Frame frame = initialize_frame(entrypoint, *main, 0, NULL);
    uint32_t *result = execute_frame(&frame);
    free(result);
    printf("ENDOFMAIN SUCCESS\n");
    return 0;
}