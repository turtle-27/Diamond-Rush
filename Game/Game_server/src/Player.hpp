#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include "TextureManager.hpp"
#include <vector>

class Player
{
    public:
		// player's collision box
		SDL_Rect playerCollider;

        // Position of player
        int player_X, player_Y;

        // player's Texture Manager
        TextureManager player_tex;

        // player's Velocity magnitude
        int PLAYER_VELOCITY = 2;

		// The velocity of the player
		int player_VelX, player_VelY;

        // Frame no. for animation
        int player_frame = 0;

        // Direction player is facing
        int player_dir = 0;

		// Player moving 
		bool player_move = false;

		// player Displaying
		bool player_display = true;

		// Invisible powerup
		bool player_power_invisble = false;

        // Vector holding player animations
        std::vector<std::vector<SDL_Rect> > player_animations;

		//Vector holding player sound effects
		std::vector<Mix_Chunk*> playerMusic;

        //Animation Speed
        int animation_speed;

        // Frames for Animation
        int NO_OF_FRAMES;
        
        //The dimensions of the player
		static const int PLAYER_WIDTH = 50;
		static const int PLAYER_HEIGHT = 50;

		//Initializes the variables
		Player();

        void load(std::string path, int x, int y, std::vector<Mix_Chunk*> music);

		//Moves the player and checks collision
		void move(std::vector<SDL_Rect> &tileSet, std::vector<SDL_Rect> &wallSet, std::vector<SDL_Rect> &coinSet,
				  std::vector<SDL_Rect> &enemies);

		//Shows the player on the screen
		void render();
};