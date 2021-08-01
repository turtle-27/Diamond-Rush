#pragma once

#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include "GameEngine.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Collision.hpp"
#include "Maze_generator.hpp"
#include "Enemy.hpp"

//Screen Constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 950;

const int MazeSizeX = 19;
const int MazeSizeY = 19;

//Tile constants
const int TILE_WIDTH = 50;
const int TILE_HEIGHT = 50;

// Player Velocity
const int VELOCITY = 2;

// Animation speed
const int animation_speed = 100;

// Player dimnesions
const int player_width = 50;
const int player_height = 50;

// Games states and menuItems
int game_state = 0;

int game_mode = 2;

int winner = -1;

int menuItem = 0;

// Player and other entities defs
int player_frame = 0;
int player_VelX = 0;
int player_VelY = 0;
bool player_display = true;
int player_invisble_time = 0;
SDL_Rect playerCollider;

int player2_frame = 0;
int player2_VelX = 0;
int player2_VelY = 0;
bool player2_display = true;
int player2_invisble_time = 0;
SDL_Rect player2Collider;

int coin_frame = 0;
int diamond_frame = 0;
bool diamond_display = true;
SDL_Rect diamondCollider;

const int INITIAL_COINS = 9;
int initial_coins1 = INITIAL_COINS;
int initial_coins2 = INITIAL_COINS;

int TOTAL_TILES = 0;
int TOTAL_WALL = 0;
int TOTAL_COINS = 0;
int TOTAL_COINS_2 = 0;
int TOTAL_GRASS = 0;

int gate_location = 0;

// SDL_Rects used
std::vector<SDL_Rect> tileSet;
std::vector<SDL_Rect> wallSet;
std::vector<SDL_Rect> grassSet;
std::vector<SDL_Rect> coinSet;
std::vector<SDL_Rect> coin2Set;

// Animations
std::vector<std::vector<SDL_Rect> > player_animations;
std::vector<std::vector<SDL_Rect> > coin_animations;
std::vector<std::vector<SDL_Rect> > diamond_animations;

// Textures Used
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

//The music that will be played
Mix_Music *ingameMusic = NULL;
Mix_Music *intenseMusic = NULL;
Mix_Music *introMusic = NULL;

//The sound effects that will be used
Mix_Chunk *coinSound = NULL;
Mix_Chunk *game_win = NULL;
Mix_Chunk *game_lose = NULL;
Mix_Chunk *death = NULL;

//The Font used
TTF_Font *gFont = NULL;

void setTiles(std::string mazefile);

// Enemies
std::vector<SDL_Rect*> enemies;
std::vector<std::pair<int,int> > enemy_spawn;