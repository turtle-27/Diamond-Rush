#include "global.hpp"
using namespace std;
maze_type conv_to_maze_type(char c)
{
    switch (c)
    {       
    case '#':
        return maze_type::wall;
        break;
    
    default:
        break;
    }
}