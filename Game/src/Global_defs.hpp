#pragma once

#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include "GameEngine.hpp"
#include "GameEngine.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Collision.hpp"
#include "Maze_generator.hpp"
#include "Enemy.hpp"

<<<<<<< HEAD

int game_state = 0;

int game_mode = 2;

=======
>>>>>>> 1cff642d90a013126609ab76826fd752a683e2b7
//Screen Constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 950;

const int MazeSizeX = 19;
const int MazeSizeY = 19;

//Tile constants
const int TILE_WIDTH = 50;
const int TILE_HEIGHT = 50;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

int animation_speed = 100;
 
int VELOCITY = 2;

int player_frame = 0;
int player_dir = 0;
bool player_move = false;
int player_X = 50;
int player_Y = 50;
int player_VelX = 0;
int player_VelY = 0;
bool player_display = true;
bool player_power_invisble = false;
int player_power_invisble_count = 3;
int player_invisble_time = 0;
int player_lifes = 3;
SDL_Rect playerCollider;

int player2_frame = 0;
int player2_dir = 0;
bool player2_move = false;
int player2_X = 50;
int player2_Y = 850;
int player2_VelX = 0;
int player2_VelY = 0;
bool player2_display = true;
bool player2_power_invisble = false;
int player2_power_invisble_count = 3;
int player2_invisble_time = 0;
int player2_lifes = 3;
SDL_Rect player2Collider;

int player_width = 50;
int player_height = 50;

int coin_frame = 0;
int diamond_frame = 0;
bool diamond_display = true;
SDL_Rect diamondCollider;

<<<<<<< HEAD
const int INITIAL_COINS = 16;

int initial_coins1 = INITIAL_COINS;
int initial_coins2 = INITIAL_COINS;
=======
int initial_coins1 = 1;
int initial_coins2 = 1;
>>>>>>> 1cff642d90a013126609ab76826fd752a683e2b7

int TOTAL_TILES = 0;
int TOTAL_WALL = 0;
int TOTAL_COINS = 0;
int TOTAL_COINS_2 = 0;
int TOTAL_GRASS = 0;

int gate_location = 0;

std::vector<SDL_Rect> tileSet;
std::vector<SDL_Rect> wallSet;
std::vector<SDL_Rect> grassSet;
std::vector<SDL_Rect> coinSet;
std::vector<SDL_Rect> coin2Set;

std::vector<std::vector<SDL_Rect> > player_animations;
std::vector<std::vector<SDL_Rect> > coin_animations;
std::vector<std::vector<SDL_Rect> > diamond_animations;

TextureManager player;
TextureManager player2;

TextureManager tileSheet;
TextureManager wallSheet;
TextureManager coinSheet;
TextureManager coin2Sheet;
TextureManager grassSheet;
TextureManager diamond;

TextureManager hud_player_life;
TextureManager hud_player2_life;
TextureManager hud_player_power_invisble_cnt;
TextureManager hud_player2_power_invisble_cnt;
TextureManager hud_X;

TextureManager hud_player;
TextureManager hud_player2;

TextureManager hud_lifes;
TextureManager hud_powerUP;

<<<<<<< HEAD
TextureManager menuImage;
TextureManager menuTitle;
TextureManager menuBG;

TextureManager menu1P;
TextureManager menu2P;
TextureManager menuExit;
TextureManager menuPointer;

TextureManager lose;
TextureManager win;

TextureManager loseText;
TextureManager winText;

TextureManager menu;
TextureManager victory;
TextureManager gameOver;

int winner = -1;

int menuItem = 0;

=======
>>>>>>> 1cff642d90a013126609ab76826fd752a683e2b7
std::string mazefile;

//The music that will be played
Mix_Music *ingameMusic = NULL;
Mix_Music *intenseMusic = NULL;
Mix_Music *introMusic = NULL;

//The sound effects that will be used
Mix_Chunk *coinSound = NULL;
Mix_Chunk *game_win = NULL;
<<<<<<< HEAD
Mix_Chunk *game_lose = NULL;
=======
>>>>>>> 1cff642d90a013126609ab76826fd752a683e2b7
Mix_Chunk *death = NULL;

//The Font used
TTF_Font *gFont = NULL;

bool Game::isRunning = false;
<<<<<<< HEAD
bool Game::quit = true;
=======
>>>>>>> 1cff642d90a013126609ab76826fd752a683e2b7

void setTiles(std::string mazefile);

std::vector<SDL_Rect*> enemies;
std::vector<std::pair<int,int> > enemy_spawn;
Enemy enemy1;
Enemy enemy2;
Enemy enemy3;
Enemy enemy4;

