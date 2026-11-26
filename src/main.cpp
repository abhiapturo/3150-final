#include <allegro5/allegro.h>
#include <iostream>
#include <string>
#include <vector>

struct GridPosition {
    int row;
    int col;
};

int main()
{
    constexpr int TILE_SIZE = 32;

    std::vector<std::string> maze = {
    "###############",
    "#P............#",
    "#.###.###.###.#",
    "#.............#",
    "#.###.#.#.###.#",
    "#.....#.#.....#",
    "###.#.....#.###",
    "#...#.G...#...#",
    "#.###.###.###.#",
    "#.............#",
    "###############"
    };

    const int mazeRows = static_cast<int>(maze.size());
    const int mazeCols = static_cast<int>(maze[0].size());

    for (const std::string& row : maze)
    {
        if (static_cast<int>(row.size()) != mazeCols)
            {
                std::cerr << "Maze rows must have equal widths.\n";
                return 1;
            }
    }

    GridPosition playerSpawn{-1, -1};
    std::vector<GridPosition> ghostSpawns;

    int playerSpawnCount = 0;
    int pelletsRemaining = 0;

    for (int row = 0; row < mazeRows; ++row){
        for (int col = 0; col < mazeCols; ++col){
            char& tile = maze[row][col];

            if (tile == 'P'){
                playerSpawn = {row, col};
                ++playerSpawnCount;

                tile = ' ';
            }
            else if (tile == 'G'){
                ghostSpawns.push_back({row, col});
                tile = ' ';
            }
            else if (tile == '.'){
                ++pelletsRemaining;
            }
        }
    }

    if (playerSpawnCount != 1 || ghostSpawns.empty()){
        std::cerr << "Maze needs exactly one player spawn and one ghost spawn";
        return -1;
    }

    std::cout << "Maze dimensions: "
          << mazeRows << " rows x "
          << mazeCols << " columns\n";

    std::cout << "Player spawn: ("
          << playerSpawn.row << ", "
          << playerSpawn.col << ")\n";

    std::cout << "Ghost count: " << ghostSpawns.size() << '\n';
    std::cout << "Pellets remaining: " << pelletsRemaining << '\n';

    // initialize Allegro
    if (!al_init())
    {
        std::cerr << "Failed to initialize Allegro!";
        return 1;
    }

    // Enable keyboard input
    if (!al_install_keyboard())
    {
        std::cerr << "Failed to enable keyboard input!";
        al_uninstall_system();
        return 1;
    }

    // Create window, timer, queue
    ALLEGRO_DISPLAY* display = al_create_display(mazeCols * TILE_SIZE, mazeRows * TILE_SIZE);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();

    if (!display || !timer || !eventQueue)
    {
        std::cerr << "Failed to create game resources!";

        if (eventQueue) al_destroy_event_queue(eventQueue);
        if (timer) al_destroy_timer(timer);
        if (display) al_destroy_display(display);

        al_uninstall_system();
        return 1;
    }

    al_set_window_title(display, "Pacman");

    // Send window, keyboard, and timer events to one queue
    al_register_event_source(
        eventQueue, al_get_display_event_source(display));

    al_register_event_source(
        eventQueue, al_get_keyboard_event_source());

    al_register_event_source(
        eventQueue, al_get_timer_event_source(timer));

    // Track game state and which keys are held
    bool running = true;
    bool redraw = true;
    bool keys[ALLEGRO_KEY_MAX] = {};

    // Timer events begin only after this call
    al_start_timer(timer);

    // Main game loop
    while (running)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);

        switch (event.type)
        {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
            {
                const int key = event.keyboard.keycode;

                if (key >= 0 && key < ALLEGRO_KEY_MAX)
                {
                    keys[key] = true;
                }

                if (key == ALLEGRO_KEY_ESCAPE)
                {
                    running = false;
                }

                // Arrow-key input checks
                if (key == ALLEGRO_KEY_UP)
                    std::cout << "Up pressed\n";
                else if (key == ALLEGRO_KEY_DOWN)
                    std::cout << "Down pressed\n";
                else if (key == ALLEGRO_KEY_LEFT)
                    std::cout << "Left pressed\n";
                else if (key == ALLEGRO_KEY_RIGHT)
                    std::cout << "Right pressed\n";

                break;
            }

            case ALLEGRO_EVENT_KEY_UP:
            {
                const int key = event.keyboard.keycode;

                if (key >= 0 && key < ALLEGRO_KEY_MAX)
                {
                    keys[key] = false;
                }

                break;
            }

            case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
                // Avoid stuck keys when switching to another window
                for (bool& keyHeld : keys)
                {
                    keyHeld = false;
                }
                break;

            case ALLEGRO_EVENT_TIMER:

                redraw = true;
                break;
        }

        // Draw once pending events have been processed.
        if (running && redraw && al_is_event_queue_empty(eventQueue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // TODO: draw the maze, player, ghosts, and score

            al_flip_display();
            redraw = false;
        }
    }

    // Clean up.
    al_stop_timer(timer);
    al_destroy_event_queue(eventQueue);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_uninstall_system();

    return 0;
}