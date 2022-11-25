#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

#ifdef X86
void *custom_calloc (size_t nmemb, size_t size) {
    if (nmemb == 0 || size == 0) {
        return NULL;
    }
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

void FURI_LOG_I(const char* tag, const char* format, ...) {
    printf("%s: %s", tag, format);
}
void println(const char* string) {
    printf("%s", string);
}

#endif
#ifndef X86
File* entrypointfile;
Application* instance;

void app_draw_callback(Canvas* canvas, void* context) {
    Application* app = context;
    FURI_LOG_I("ZERO_JVM", "get mutex");
    furi_check(furi_mutex_acquire(app->model_mutex, FuriWaitForever) == FuriStatusOk);
    FURI_LOG_I("ZERO_JVM", "received mutex");
    canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignCenter, app->display_text);
    FURI_LOG_I("ZERO_JVM", "release mutex");
    furi_mutex_release(app->model_mutex);
    FURI_LOG_I("ZERO_JVM", "mutex released");
}

Application* app_allocator() {
    Application* app = malloc(sizeof(Application));
    app->model_mutex = furi_mutex_alloc(FuriMutexTypeNormal);
    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, app_draw_callback, app);
    app->gui = furi_record_open("gui");
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);
    return app;
}

void println(const char* string) {
    printf("%s", string);
    FURI_LOG_I("ZERO_JVM", "Enter println");
    instance->display_text = malloc(strlen(string) + 1);
    strcpy(instance->display_text, string);
    view_port_update(instance->view_port);
    FURI_LOG_I("ZERO_JVM", "Exit println");
}

void app_deallocator() {
    view_port_enabled_set(instance->view_port, 0); // Disables our ViewPort
    gui_remove_view_port(instance->gui, instance->view_port); // Removes our ViewPort from the Gui
    furi_record_close("gui"); // Closes the gui record
    view_port_free(instance->view_port); // Frees memory allocated by view_port_alloc
    furi_mutex_free(instance->model_mutex);
    free(instance->display_text);
    free(instance);
}


void *custom_calloc (size_t nmemb, size_t size) {
    if (nmemb == 0 || size == 0) {
        return NULL;
    }
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