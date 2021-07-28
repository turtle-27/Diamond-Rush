#pragma once
#include <SDL2/SDL.h>
#include "Map.hpp"


class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 1;

		//Initializes the variables
		Dot();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot and checks collision
		void move( Tile* tiles[], int screenW, int screenH);

		//Shows the dot on the screen
		void render();

    private:
		//Dot's collision box
		SDL_Rect mBox;
		
		//The velocity of the dot
		int mVelX, mVelY;
};