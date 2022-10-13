#include <furi.h>
#include <gui/gui.h>
#include <stdlib.h>
#include <errno.h>

int32_t zero_jvm_entrypoint(void* p) {
    UNUSED(p);
    return 1;
}