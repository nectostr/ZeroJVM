#include <furi.h>
#include <gui/gui.h>
#include <stdlib.h>
#include "libpassgen_embedded.h"
#include <errno.h>

typedef struct {
    FuriMutex* app_mutex;
    FuriMessageQueue* event_queue;

    ViewPort* view_port;
    Gui* gui;

    uint32_t password_length;
    char* display_text;

} Application;

void draw_callback(Canvas* canvas, void* ctx) {
    Application* app = ctx;
    furi_check(furi_mutex_acquire(app->app_mutex, FuriWaitForever) == FuriStatusOk);

    canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignCenter, (char*)app->display_text);

    furi_mutex_release(app->app_mutex);
}

void input_callback(InputEvent* input, void* ctx) {
    Application* app = ctx;
    furi_message_queue_put(app->event_queue, input, FuriWaitForever);
}

Application* app_allocator() {
    FURI_LOG_T("password-generator", "Allocating memory...");
    Application* instance = malloc(sizeof(Application));

    instance->view_port = view_port_alloc();
    view_port_draw_callback_set(instance->view_port, draw_callback, instance);
    view_port_input_callback_set(instance->view_port, input_callback, instance);

    instance->password_length = 12;
    instance->app_mutex = furi_mutex_alloc(FuriMutexTypeNormal);

    instance->gui = furi_record_open("gui");
    instance->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    gui_add_view_port(instance->gui, instance->view_port, GuiLayerFullscreen);
    FURI_LOG_T("password-generator", "Memory allocated");

    return instance;
}

void app_free(Application* instance) {
    view_port_enabled_set(instance->view_port, false); // Disables our ViewPort
    gui_remove_view_port(instance->gui, instance->view_port); // Removes our ViewPort from the Gui
    furi_record_close("gui"); // Closes the gui record
    view_port_free(instance->view_port); // Frees memory allocated by view_port_alloc

    furi_message_queue_free(instance->event_queue);
    furi_mutex_free(instance->app_mutex);
    free(instance->display_text);
    free(instance);
}

int32_t passgen_entrypoint_app(void* p) {
    UNUSED(p);
    Application* instance = app_allocator();

    InputEvent event;
    for(bool processing = true; processing;) {
        FURI_LOG_T("password-generator", "Waiting for ");
        int status = furi_message_queue_get(instance->event_queue, &event, 100);
        furi_check(furi_mutex_acquire(instance->app_mutex, FuriWaitForever) == FuriStatusOk);

        if(status == FuriStatusOk) {
            FURI_LOG_D("password-generator", "Event captured");
            if(event.type == InputTypePress) {
                FURI_LOG_D("password-generator", "Input captured");
                switch(event.key) {
                    case InputKeyOk: {
                        FURI_LOG_D("password-generator", "InputKeyOK pressed");
                        // do something
                        uint64_t random_state;
                        random_state = (uint64_t)rand();
                        FURI_LOG_T("password-generator", "Random result generated");
                        GenerationResult generation_result;
                        instance->display_text =
                                malloc((instance->password_length + 1) * sizeof(char));
                        FURI_LOG_T("password-generator", "Password generation started");
                        generation_result = generate_password(
                                (uint8_t*)instance->display_text, instance->password_length, random_state);

                        FURI_LOG_D("password-generator", "Generation finished");

                        // // if failure: realloc memory and copy str to display text, if success: do nothing
                        if(!generation_result.success) {
                            FURI_LOG_D("password-generator", "Generation result: failure");
                            char* error_string = (char*)generation_result.error_string;
                            free(instance->display_text);
                            instance->display_text = malloc(strlen(error_string) * sizeof(char));
                            strcpy(instance->display_text, error_string);
                        }

                        break;
                    }
                    case InputKeyBack:
                        FURI_LOG_D("password-generator", "InputKeyBack pressed");
                        processing = false;
                        break;
                    case InputKeyUp:
                    case InputKeyDown:
                    case InputKeyLeft:
                    case InputKeyRight:
                        break;
                }
            }
        }

        furi_mutex_release(instance->app_mutex);
        view_port_update(instance->view_port);
    }
    app_free(instance);
    return 0;
}