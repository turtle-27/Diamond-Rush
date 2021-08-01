#pragma once
#include <SDL2/SDL.h>
#include "TextureManager.hpp"

//The tile
class Tile
{
    public:
		//Initializes position and type
		Tile( int x, int y, int tileType, int TILE_WIDTH, int TILE_HEIGHT );

		//Shows the tile
		void render();

		//Get the tile type
		int getType();

		//Get the collision box
		SDL_Rect getBox();

    private:
		//The attributes of the tile
		SDL_Rect mBox;

		//The tile type
		int mType;
};