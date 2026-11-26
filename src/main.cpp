#include <iostream>
#include <allegro5/allegro.h>

int main()
{
    // initialize Allegro
    if (!al_init()){
        std::cerr << "Failed to initialize Allegro!";
        return 1;
    }


    // Create an 800 x 600 Allegro window
    ALLEGRO_DISPLAY* display = al_create_display(800, 600);

    if (display == nullptr){
        std::cerr << "Failed to create the display";
        al_uninstall_system();
        return 1;
    }

    al_set_window_title(display, "Pacman");


    // Create an queue to receive events
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();

    if (eventQueue == nullptr)
    {
        std::cerr << "Failed to create queue";
        al_destroy_display(display);
        al_uninstall_system();
        return 1;
    }

    al_register_event_source(
        eventQueue,
        al_get_display_event_source(display)
    );

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    // Maintain the game loop until user exits the window
    bool running = true;

    while (running)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            running = false;
        }
    }

    al_destroy_event_queue(eventQueue);
    al_destroy_display(display);
    al_uninstall_system();

    return 0;
}

