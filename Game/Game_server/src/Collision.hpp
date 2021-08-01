#pragma once

#include <SDL2/SDL.h>
#include <vector>

bool checkCollision( SDL_Rect a, SDL_Rect b );

bool touchesWall( SDL_Rect box, std::vector<SDL_Rect> tiles_vc);

bool touchesCoin( SDL_Rect box, std::vector<SDL_Rect> coins_vc);