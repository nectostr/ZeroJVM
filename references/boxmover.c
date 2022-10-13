#include <furi.h>
#include <gui/gui.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} BoxMoverModel;

typedef struct {
    BoxMoverModel* model;
    FuriMutex* model_mutex;

    FuriMessageQueue* event_queue;

    ViewPort* view_port;
    Gui* gui;

} BoxMover;

void draw_callback(Canvas* canvas, void* ctx) {
    BoxMover* box_mover = ctx;
    furi_check(furi_mutex_acquire(box_mover->model_mutex, FuriWaitForever) == FuriStatusOk);

    canvas_draw_box(
            canvas, box_mover->model->x, box_mover->model->y, 4, 4); // Draw a box on the screen

    furi_mutex_release(box_mover->model_mutex);
}

void input_callback(InputEvent* input, void* ctx) {
    BoxMover* box_mover = ctx;
    furi_message_queue_put(box_mover->event_queue, input, FuriWaitForever);
}

BoxMover* box_mover_alloc() {
    BoxMover* instance = malloc(sizeof(BoxMover));
    instance->model = malloc(sizeof(BoxMoverModel));
    instance->model->x = 10;
    instance->model->y = 10;

    instance->view_port = view_port_alloc();
    view_port_draw_callback_set(instance->view_port, draw_callback, instance);
    view_port_input_callback_set(instance->view_port, input_callback, instance);

    instance->model_mutex = furi_mutex_alloc(FuriMutexTypeNormal);

    instance->gui = furi_record_open("gui");
    instance->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    gui_add_view_port(instance->gui, instance->view_port, GuiLayerFullscreen);

    return instance;
}

void box_mover_free(BoxMover* instance) {
    view_port_enabled_set(instance->view_port, false); // Disables our ViewPort
    gui_remove_view_port(instance->gui, instance->view_port); // Removes our ViewPort from the Gui
    furi_record_close("gui"); // Closes the gui record
    view_port_free(instance->view_port); // Frees memory allocated by view_port_alloc

    furi_message_queue_free(instance->event_queue);
    furi_mutex_free(instance->model_mutex);
    free(instance->model);
    free(instance);
}

int32_t box_mover_app(void* p) {
    UNUSED(p);
    BoxMover* box_mover = box_mover_alloc();

    InputEvent event;
    for(bool processing = true; processing;) {
        // Pops a message off the queue and stores it in `event`.
        // No message priority denoted by NULL, and 100 ticks of timeout.
        int status = furi_message_queue_get(box_mover->event_queue, &event, 100);
        furi_check(furi_mutex_acquire(box_mover->model_mutex, FuriWaitForever) == FuriStatusOk);
        if(status == FuriStatusOk) {
            if(event.type == InputTypePress) {
                switch(event.key) {
                    case InputKeyUp:
                        box_mover->model->y -= 2;
                        break;
                    case InputKeyDown:
                        box_mover->model->y += 2;
                        break;
                    case InputKeyLeft:
                        box_mover->model->x -= 2;
                        break;
                    case InputKeyRight:
                        box_mover->model->x += 2;
                        break;
                    case InputKeyOk:
                    case InputKeyBack:
                        processing = false;
                        break;
                }
            }
        }
        furi_mutex_release(box_mover->model_mutex);
        view_port_update(box_mover->view_port); // signals our draw callback
    }

    box_mover_free(box_mover);
    return 0;
}