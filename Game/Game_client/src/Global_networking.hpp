#pragma once 

#include <string>
#include <SDL2/SDL.h>
#include <vector>
#include "Enemy.hpp"


struct glob
{

std::string mazefile;

int player_dir = 0;
bool player_move = false;
int player_X = 50;
int player_Y = 50;
int player_lifes = 3;
bool player_power_invisble = false;
int player_power_invisble_count = 3;

int player2_dir = 0;
bool player2_move = false;
int player2_X = 50;
int player2_Y = 850;
int player2_lifes = 3;
bool player2_power_invisble = false;
int player2_power_invisble_count = 3;

Enemy enemy1;
Enemy enemy2;
Enemy enemy3;
Enemy enemy4;

};