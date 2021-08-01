#include <iostream>

#include "Player.hpp"
#include "Collision.hpp"


Player::Player()
{
    //Initialize the velocity
    player_VelX = 0;
    player_VelY = 0;

    // Initialize animation speed
    animation_speed = 100;

    // Initialize no of frames for animation 
    NO_OF_FRAMES = 4;

    // Player Animations
    for (int i = 0; i < 4; i++)
    {
        std::vector<SDL_Rect> row;
        for (int j = 0; j < 4; j++)
        {
            SDL_Rect src;
            src.x = j * 64;
            src.y = i * 64; 
            src.w = 64;
            src.h = 64;
            row.push_back(src);
        }
        player_animations.push_back(row);
    }
}

void Player::load(std::string path, int x, int y, std::vector<Mix_Chunk*> music)
{
    // Initial enemy position
    player_X = x;
    player_Y = y;
    
    // Colliders for the players
    playerCollider.x = player_X + PLAYER_WIDTH/8;
    playerCollider.y = player_Y + PLAYER_HEIGHT/8;
    playerCollider.w = 3*PLAYER_WIDTH/4;
    playerCollider.h = 3*PLAYER_HEIGHT/4;

    playerMusic = music;

    player_tex.loadFromFile(path);
}

void Player::move(std::vector<SDL_Rect> &tileSet, std::vector<SDL_Rect> &wallSet, std::vector<SDL_Rect> &coinSet,
				  std::vector<SDL_Rect> &enemies)
{   
    if (player_move)
    {
        player_frame = (SDL_GetTicks() / animation_speed) % 4;
    }

    player_X += player_VelX;
    playerCollider.x += player_VelX;
    
    if( ( touchesWall( playerCollider, tileSet) || 
          touchesWall( playerCollider, wallSet)))
    {
        player_X -= player_VelX;
        playerCollider.x -= player_VelX;
    }

    player_Y += player_VelY;
    playerCollider.y += player_VelY;

    if( touchesWall( playerCollider, tileSet  ) || 
        touchesWall( playerCollider, wallSet ) )
    {
        player_Y -= player_VelY;
        playerCollider.y -= player_VelY;
    }

    for( int i = 0; i < coinSet.size(); i++ )
    {
        //If the collision box touches the coin 
        if( checkCollision( playerCollider, coinSet[ i ] ) )
        {
            coinSet.erase(coinSet.begin() + i);
            Mix_PlayChannel(-1, playerMusic[0], 0);
            break;
        }
    }
}


void Player::render()
{    
    if (player_move)
    {
        player_frame = (SDL_GetTicks() / animation_speed) % 4;
    }
    
    SDL_Rect* player_currentClip = &player_animations[player_dir][player_frame];
    if (player_display)
    {
        player_tex.render( player_X, player_Y, player_currentClip, PLAYER_WIDTH, PLAYER_HEIGHT, 1);
    }
}
