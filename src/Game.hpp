#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <vector>

class ColliderComponent;

class Game {
    public:
        Game();
        ~Game();

        void init(const char* title, int x, int y, int width, int height, bool fullScreen);

        void handleEvents();
        void update();
        bool running(){return isRunning;}
        void render();
        void clean();

        static SDL_Renderer* renderer;
        static SDL_Event event;
        static bool isRunning;
        static SDL_Rect camera;

        enum groupLabels : std::size_t {
            groupMap,
            groupPlayers,
            groupEnemies,
            groupColliders
        };
    private:
        int count = 0;

        SDL_Window *window;
};