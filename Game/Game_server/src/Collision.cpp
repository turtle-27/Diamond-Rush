#include "Collision.hpp"

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}


bool touchesWall( SDL_Rect box, std::vector<SDL_Rect> tiles_vc)
{
    //Go through the tiles
    for( int i = 0; i < tiles_vc.size(); i++ )
    {
        //If the collision box touches the wall tile
        if( checkCollision( box, tiles_vc[ i ] ) )
        {
            return true;
        }
    }

    //If no wall tiles were touched
    return false;
}

bool touchesCoin( SDL_Rect box, std::vector<SDL_Rect> coins_vc)
{
    //Go through the coins
    for( int i = 0; i < coins_vc.size(); i++ )
    {
        //If the collision box touches the wall tile
        if( checkCollision( box, coins_vc[ i ] ) )
        {
            return true;
        }
    }

    //If no coins were touched
    return false;
}
