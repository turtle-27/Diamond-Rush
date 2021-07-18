#include "map.hpp"
#include "game.hpp"
#include <fstream>

Map::Map()
{
    
}

Map::~Map()
{
  
}

void  Map::LoadMap(std::string path, int sizeX, int sizeY)
{
    char tile;
    std::fstream mapFile;
    mapFile.open(path);

    for (int y = 0; y < sizeY; y++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            mapFile.get(tile);
            Game::AddTile(atoi(&tile), x * 32, y * 32);
            std::cout << atoi(&tile) << std::endl;
            // mapFile.ignore();
        }
        mapFile.ignore();
    }
    mapFile.close();
}