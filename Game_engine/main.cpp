#include "game.hpp"

Game *game = nullptr;

int main(int argc, const char* argv[])
{
    const int FPS = 60;
    const int time_per_frame = 1000/FPS; 

    Uint32 frameStart;
    int frameTime;
    
    game = new Game();
    game->init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

    while(game->running())
    {
        
        frameStart = SDL_GetTicks();
        
        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;

        if (time_per_frame > frameTime)
        {
            SDL_Delay(time_per_frame - frameTime);
        }
    }

    game->clean();

    return 0;
}