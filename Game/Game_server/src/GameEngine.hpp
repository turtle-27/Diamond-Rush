#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>

class Game 
{
    public:
        Game();
        ~Game();

        void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
        void loadMedia();
        void handleEvents();
        void update();
        void render();
        void clean();
        static struct glob* getStruct();

        bool running() { return isRunning; }

        static SDL_Renderer* renderer; 
        static SDL_Event event; 
        static bool isRunning;
        static bool quit;
        
    private:
        int cnt = 0;
        
        SDL_Window *window;
};