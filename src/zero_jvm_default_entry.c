#include <stdint.h>
#include "zero_jvm/loader.c"

int32_t main(int argc, char** argv) {
    return read_class();
}