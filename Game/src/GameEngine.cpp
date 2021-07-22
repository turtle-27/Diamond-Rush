#include "GameEngine.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include <string>
#include <map>
#include <vector>
#include <fstream>

//Screen Constants
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

const int MazeSizeX = 37;
const int MazeSizeY = 37;

//Tile constants
const int TILE_WIDTH = 27;
const int TILE_HEIGHT = 27;
const int TOTAL_TILE_SPRITES = 1;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

int animation_speed = 100;
int player_frame = 0;
int player_dir = 0;
bool player_move = false;

int player2_frame = 0;
int player2_dir = 0;
bool player2_move = false;

int TOTAL_TILES = 0;
SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];
std::vector<SDL_Rect> tileSet;
std::vector<std::vector<SDL_Rect> > animations;
TextureManager player;
TextureManager player2;
TextureManager tileSheet;


bool Game::isRunning = false;

Game::Game()
{}
Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        renderer = SDL_CreateRenderer(window, - 1,  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }

        isRunning = true;
    }
}

bool setTiles()
{
	//Success flag
	bool tilesLoaded = true;

    //Open the map
    std::ifstream map( "data/mazefile.txt" );

    //If the map couldn't be loaded
    if( map.fail() )
    {
		printf( "Unable to load map file!\n" );
		tilesLoaded = false;
    }
	else
	{
        //Initialize the tiles
		for( int x = 0; x < MazeSizeX; x++ )
		{
			for ( int y = 0; y < MazeSizeY; y++ )
            {
                //Determines what kind of tile will be made
			    int tileType = -1;

			    //Read tile from map file
			    map >> tileType;
			    //If the was a problem in reading the map
			    if( map.fail() )
			    {
			    	//Stop loading map
			    	printf( "Error loading map: Unexpected end of file!\n" );
			    	tilesLoaded = false;
			    	break;
			    }

			    //If the number is a valid tile number
			    if( tileType == 1 )
			    {
                    SDL_Rect newTile;
                    newTile.x = x * TILE_WIDTH;
                    newTile.y = y * TILE_HEIGHT;
                    newTile.w = TILE_WIDTH;
                    newTile.h = TILE_HEIGHT;
                    tileSet.push_back(newTile);
                    TOTAL_TILES++;
			    }
			    // //If we don't recognize the tile type
			    // else
			    // {
			    // 	//Stop loading map
			    // 	printf( "Error loading map: Invalid tile type at %d!\n", TOTAL_TILES );
			    // 	tilesLoaded = false;
			    // 	break;
			    // }
            }
		}
		
		//Clip the sprite sheet
		if( tilesLoaded )
		{
			gTileClips[ 0 ].x = 0;
			gTileClips[ 0 ].y = 0;
			gTileClips[ 0 ].w = TILE_WIDTH;
			gTileClips[ 0 ].h = TILE_HEIGHT;
		}
	}

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}

void Game::loadMedia()
{
    player.loadFromFile("data/player.png");
    player.setColor(150, 120, 0);
    player2.loadFromFile("data/player.png");
    player2.setColor(0, 150, 150);
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
        animations.push_back(row);
    }

    tileSheet.loadFromFile("data/wall.png");
    setTiles() ;
}

void Game::handleEvents()
{
    while(SDL_PollEvent( &Game::event ) != 0)
    {
        if( Game::event.type == SDL_QUIT )
	    {
	    	Game::isRunning = false;
	    }
        else if (Game::event.type == SDL_KEYDOWN)
        {
            
            switch(Game::event.key.keysym.sym)
            {
                case SDLK_w:
                    player_dir = 3;
                    player_move = true;
                    break;
                case SDLK_a:
                    player_dir = 1; 
                    player_move = true;
                    break;
                case SDLK_d:
                    player_dir = 2;
                    player_move = true;
                    break; 
                case SDLK_s:
                    player_dir = 0;
                    player_move = true;
                    break; 
                case SDLK_UP:
                    player2_dir = 3;
                    player2_move = true;
                    break;
                case SDLK_DOWN:
                    player2_dir = 0; 
                    player2_move = true;
                    break;
                case SDLK_LEFT:
                    player2_dir = 1;
                    player2_move = true;
                    break; 
                case SDLK_RIGHT:
                    player2_dir = 2;
                    player2_move = true;
                    break;     
                case SDLK_ESCAPE:
                    Game::isRunning = false;
                default:
                    break;      
            }
        }
        else if(Game::event.type == SDL_KEYUP)
        {
            switch(Game::event.key.keysym.sym)
            {
                case SDLK_w:
                    player_move = false;
                    player_frame = 0;
                    break;
                case SDLK_a:
                    player_move = false;
                    player_frame = 0;
                    break;
                case SDLK_d:
                    player_move = false;
                    player_frame = 0;
                    break; 
                case SDLK_s:
                    player_move = false;
                    player_frame = 0;
                    break;
                case SDLK_UP:
                    player2_move = false;
                    player2_frame = 0;
                    break;
                case SDLK_DOWN:
                    player2_move = false;
                    player2_frame = 0;
                    break;
                case SDLK_LEFT:
                    player2_move = false;
                    player2_frame = 0;
                    break; 
                case SDLK_RIGHT:
                    player2_move = false;
                    player2_frame = 0;
                    break;      
                default:
                    break;      
            }
        }
    }
}

void Game::update()
{
    if (player_move)
    {
        player_frame = (SDL_GetTicks() / animation_speed) % 4;
    }

    if (player2_move)
    {
        player2_frame = (SDL_GetTicks() / animation_speed) % 4;
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0xFF );
    SDL_RenderClear(renderer);
    
    SDL_Rect* player_currentClip = &animations[player_dir][player_frame];
    SDL_Rect* player2_currentClip = &animations[player2_dir][player2_frame];	
    
    for( int i = 0; i < TOTAL_TILES; ++i )
	{
        tileSheet.render(tileSet[i].x, tileSet[i].y, NULL, 32, 32, 1);
	}

    player.render(( SCREEN_WIDTH - player_currentClip->w ) / 2, ( SCREEN_HEIGHT - player_currentClip->h ) / 2, player_currentClip, 32, 32, 2);
    player2.render(( SCREEN_WIDTH - player2_currentClip->w ) / 4, ( SCREEN_HEIGHT - player2_currentClip->h ) / 4, player2_currentClip, 32, 32, 2);
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    // Free loaded images
    player.free();
    tileSheet.free();

    //Deallocate tiles
	for( int i = 0; i < TOTAL_TILES; ++i )
	{
        tileSet.pop_back();
	}
   
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_Quit;
    IMG_Quit;
    SDL_Quit();
}