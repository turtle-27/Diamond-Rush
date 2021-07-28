#include <iostream>

#include "Enemy.hpp"
#include "Collision.hpp"

Enemy::Enemy()
{
    // Initialize animation speed
    animation_speed = 100;

    // Initialize no of frames for animation 
    NO_OF_FRAMES = 6;

    // Enemy Animations
    for (int i = 0; i < 4; i++)
    {
        std::vector<SDL_Rect> row;
        for (int j = 0; j < NO_OF_FRAMES; j++)
        {
            SDL_Rect src;
            src.x = j * 48;
            src.y = i * 48; 
            src.w = 48;
            src.h = 48;
            row.push_back(src);
        }
        enemy_animations.push_back(row);
    }
}

void Enemy::load(std::string path, int x, int y, int vel)
{
    // Initial enemy position
    enemy_X = x;
    enemy_Y = y;
    
    ENEMY_VELOCITY = vel;
    
    // Initialize the x and y velocity
    enemy_VelX = ENEMY_VELOCITY;
    enemy_VelY = ENEMY_VELOCITY;

    // Colliders for the enemy
    enemyCollider.x = enemy_X + ENEMY_WIDTH/8;
    enemyCollider.y = enemy_Y + ENEMY_HEIGHT/8;
    enemyCollider.w = 3*ENEMY_WIDTH/4;
	enemyCollider.h = 3*ENEMY_HEIGHT/4;

    enemy_tex.loadFromFile(path);
}

int changeTime = 0;

void Enemy::move(std::vector<SDL_Rect> tileSet, std::vector<SDL_Rect> wallSet)
{
    bool check_enemy_collide = false;
    
    if (SDL_GetTicks() - changeTime >= 3000)
    {
        enemy_dir = rand() % 4;
        
        if( enemy_dir == 3 )
	    {
            enemy_VelY = -ENEMY_VELOCITY;
            enemy_VelX = 0;
	    }
	    else if( enemy_dir == 0 )
	    {   
            enemy_VelY = ENEMY_VELOCITY;
            enemy_VelX = 0;
	    }
	    else if( enemy_dir == 1 )
	    {
            enemy_VelX = -ENEMY_VELOCITY;
            enemy_VelY = 0;
	    }
	    else if( enemy_dir == 2 )
	    {   
            enemy_VelX = ENEMY_VELOCITY;
            enemy_VelY = 0;
	    }
        changeTime = SDL_GetTicks();
    }

    enemy_X += enemy_VelX;
    enemyCollider.x += enemy_VelX;

    if( touchesWall( enemyCollider, tileSet ) || touchesWall( enemyCollider, wallSet) )
    {
        enemy_X -= enemy_VelX;
        enemyCollider.x -= enemy_VelX;
        check_enemy_collide = true;
    }

    enemy_Y += enemy_VelY;
    enemyCollider.y += enemy_VelY;

    if( touchesWall( enemyCollider, tileSet) || touchesWall( enemyCollider, wallSet) )
    {
        enemy_Y -= enemy_VelY;
        enemyCollider.y -= enemy_VelY;
        check_enemy_collide = true;
    }
    
    while(check_enemy_collide)
    {
        check_enemy_collide = false;

        enemy_dir = rand() % 4;

        if( enemy_dir == 3 )
	    {
            enemy_VelY = -ENEMY_VELOCITY;
            enemy_VelX = 0;
	    }
	    else if( enemy_dir == 0 )
	    {   
            enemy_VelY = ENEMY_VELOCITY;
            enemy_VelX = 0;
	    }
	    else if( enemy_dir == 1 )
	    {
            enemy_VelX = -ENEMY_VELOCITY;
            enemy_VelY = 0;
	    }
	    else if( enemy_dir == 2 )
	    {   
            enemy_VelX = ENEMY_VELOCITY;
            enemy_VelY = 0;
	    }
        
        enemy_X += enemy_VelX;
        enemyCollider.x += enemy_VelX;

        if( touchesWall( enemyCollider, tileSet) || touchesWall( enemyCollider, wallSet) )
        {
            enemy_X -= enemy_VelX;
            enemyCollider.x -= enemy_VelX;
            check_enemy_collide = true;
        }

        enemy_Y += enemy_VelY;
        enemyCollider.y += enemy_VelY;

       if( touchesWall( enemyCollider, tileSet) || touchesWall( enemyCollider, wallSet ) )
        {
            enemy_Y -= enemy_VelY;
            enemyCollider.y -= enemy_VelY;
            check_enemy_collide = true;
        }
    }
}

void Enemy::render()
{
    enemy_frame = (SDL_GetTicks() / animation_speed) % NO_OF_FRAMES;
    
    SDL_Rect* enemy_currentClip = &enemy_animations[enemy_dir][enemy_frame];
    enemy_tex.render( enemy_X, enemy_Y, enemy_currentClip, ENEMY_WIDTH, ENEMY_HEIGHT, 1);
}