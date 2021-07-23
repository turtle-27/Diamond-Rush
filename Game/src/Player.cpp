#include "Player.hpp"
// #include "Global_defs.hpp"
// #include "Collision.hpp"

Dot::Dot()
{
	//Set collision box dimension
	mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

// bool touchesWall( SDL_Rect box, Tile* tiles[] )
// {
//     //Go through the tiles
//     for( int i = 0; i < TOTAL_TILES; ++i )
//     {
//         //If the collision box touches the wall tile
//         if( checkCollision( box, tiles[ i ]->getBox() ) )
//         {
//             return true;
//         }
//     }

//     //If no wall tiles were touched
//     return false;
// }

// void Dot::move( Tile *tiles[] )
// {
//     //Move the dot left or right
//     mBox.x += mVelX;

//     //If the dot went too far to the left or right or touched a wall
//     if( ( mBox.x < 0 ) || ( mBox.x + DOT_WIDTH > SCREEN_WIDTH ) || touchesWall( mBox, tiles ) )
//     {
//         //move back
//         mBox.x -= mVelX;
//     }

//     //Move the dot up or down
//     mBox.y += mVelY;

//     //If the dot went too far up or down or touched a wall
//     if( ( mBox.y < 0 ) || ( mBox.y + DOT_HEIGHT > SCREEN_HEIGHT ) || touchesWall( mBox, tiles ) )
//     {
//         //move back
//         mBox.y -= mVelY;
//     }
// }

void Dot::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}


