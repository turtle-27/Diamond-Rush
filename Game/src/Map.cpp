#include "Map.hpp"

Tile::Tile( int x, int y, int tileType, int TILE_WIDTH, int TILE_HEIGHT  )
{
    //Get the offsets
    mBox.x = x;
    mBox.y = y;

    //Set the collision box
    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;

    //Get the tile type
    mType = tileType;

    // Assign pointer to corresponding texture
}

// void Tile::render(TextureManager TileTex, SDL_Rect gTileClips[])
// {
//     //If the tile is on screen
//     // if( checkCollision( camera, mBox ) )
//     // {
//     // }
//     //Show the tile
//     TileTex.render( mBox.x , mBox.y , &gTileClips[ mType ], );
// }

int Tile::getType()
{
    return mType;
}

SDL_Rect Tile::getBox()
{
    return mBox;
}