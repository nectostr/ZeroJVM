#include "runtime.h"

Runtime runtime;

void init_runtime() {
    runtime.max_statics_map_index = 0;
    runtime.statics_map = custom_calloc(STATICS_MAP_SIZE, sizeof(MapEntry));

    runtime.max_statics_table_offset = 0;
    runtime.statics_table = custom_calloc(STATICS_TABLE_SIZE, sizeof(uint8_t));
}
