#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>

#include <time.h>
#include <stdlib.h>
#include <math.h>

class Game {
    SDL_Window *window;
    SDL_Event e;
    bool running;

    Uint32 game_tick;
public:
    Game(const char*, int, int);
    ~Game();

    static SDL_Renderer *renderer;

    bool isRunning();
    void update();
    void render();
    void event_loop();

    SDL_Window* getWindow();

    friend class Interface;
};

#endif // GAME_H_
