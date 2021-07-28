#pragma once

#include <SDL2/SDL.h>
#include <string>
#include "TextureManager.hpp"
#include <vector>


class Enemy
{
    public:
		// Enemy's collision box
		SDL_Rect enemyCollider;

        // Position of enemy
        int enemy_X, enemy_Y;

        // Enemy's Texture Manager
        TextureManager enemy_tex;

        // Enemy's Velocity magnitude
        int ENEMY_VELOCITY;

		// The velocity of the enemy
		int enemy_VelX, enemy_VelY;

        // Frame no. for animation
        int enemy_frame = 0;

        // Direction enemy is facing
        int enemy_dir = 1;

        // Vector holding enemy animations
        std::vector<std::vector<SDL_Rect> > enemy_animations;

        //Animation Speed
        int animation_speed;

        // Frames for Animation
        int NO_OF_FRAMES;
        
        //The dimensions of the enemy
		static const int ENEMY_WIDTH = 50;
		static const int ENEMY_HEIGHT = 50;

		//Initializes the variables
		Enemy();

        // Loads the texture
        void load(std::string path, int x, int y, int vel);

		//Moves the enemy and checks collision
		void move(std::vector<SDL_Rect> tileSet, std::vector<SDL_Rect> wallSet);

		//Shows the enemy on the screen
		void render();
};