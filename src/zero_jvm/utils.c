#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

#ifdef X86
void *custom_calloc (size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}
FILE *custom_fopen (const char *__restrict filename, const char *__restrict modes) {
    return fopen(filename, modes);
}
int custom_fclose (FILE *stream) {
    return fclose(stream);
}
size_t custom_fread (void *__restrict ptr, size_t size, size_t n, FILE *__restrict stream) {
    return fread(ptr, size, n, stream);
}
#endif
#ifndef X86
File* entrypointfile;

void *custom_calloc (size_t nmemb, size_t size) {
    return malloc(nmemb * size);  // on flipper zero malloc == calloc
}
FILE *custom_fopen (const char *__restrict filename, const char *__restrict modes) {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    entrypointfile = storage_file_alloc(storage);
    UNUSED(modes);
    storage_file_open(entrypointfile, filename, FSAM_READ, FSOM_OPEN_EXISTING);
    return NULL;
}
int custom_fclose (FILE *stream) {
    UNUSED(stream);
    storage_file_close(entrypointfile);
    return 0;
}
size_t custom_fread (void *__restrict ptr, size_t size, size_t n, FILE *__restrict stream) {
    UNUSED(stream);
    return storage_file_read(entrypointfile, ptr, size * n);
}
#endif


char *combine_names_with_dot(char *firstname, char *lastname) {
    unsigned long fullmethodnamelen = strlen(firstname) + strlen(lastname) + 2;
    char *fullname = custom_calloc(fullmethodnamelen, sizeof(char));
    strcpy(fullname, firstname);
    fullname[strlen(firstname)] = '.';
    strcpy(&fullname[strlen(firstname) + 1], lastname);
    return fullname;
}