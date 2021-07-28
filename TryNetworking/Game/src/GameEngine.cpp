#include <string>
#include <map>
#include <vector>
#include <fstream>
#include "GameEngine.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "Global_defs.hpp"
// #include "Player.hpp"
#include "Collision.hpp"

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

int animation_speed = 100;


int player_frame = 0;
int player_dir = 0;
bool player_move = false;
int player_X = 64;
int player_Y = 64;
int player_VelX = 0;
int player_VelY = 0;
int player_width = 64;
int player_height = 64;
SDL_Rect playerCollider;

int player2_frame = 0;
int player2_dir = 0;
bool player2_move = false;
int player2_X = 500;
int player2_Y = 500;
int player2_VelX = 0;
int player2_VelY = 0;


SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];
std::vector<SDL_Rect> tileSet;
std::vector<std::vector<SDL_Rect> > animations;
TextureManager player;
TextureManager player2;
TextureManager tileSheet;

// Dot dot;

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

void setTiles()
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
            }
		}
	}

    //Close the file
    map.close();
}

bool touchesWall( SDL_Rect box, std::vector<SDL_Rect> tiles_vc)
{
    //Go through the tiles
    for( int i = 0; i < TOTAL_TILES; i++ )
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

void Game::loadMedia()
{
    player.loadFromFile("data/player.png");
    player.setColor(150, 120, 0);
    player2.loadFromFile("data/player.png");
    player2.setColor(50, 150, 120);
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
    setTiles();

    
    playerCollider.x = player_X + player_width/8;
    playerCollider.y = player_Y + player_height/8;
    playerCollider.w = 3*player_width/4;
    playerCollider.h = 3*player_height/4;
}

void Game::handleEvents(char* text, int ind)
{
    // input_struct1 inp_str_game;
    // input_struct2 inp_str_game2;
    // char* c1 = inp_str_game.text_p1;
    // char* c2 = inp_str_game2.text_p2;
    
    if (ind == 0)
    {
        std::cout << "ind " << ind << std::endl;
        //std::cout << "180" << std::endl;
        player2_move = false;
        if (*text == 'w')
        {
            //std::cout << "183.Game" << std::endl;
            player_dir = 3;
            player_move = true;
        }
        else if (*text == 's')
        {
            player_dir = 0;
            player_move = true;
        }
        else
        {
            player_move = false;
        }
    }
    else
    {
        std::cout << "ind " << ind << std::endl;
        player_move = false;
        if (*text == 'i')
        {
            //std::cout << "183.Game" << std::endl;
            player2_dir = 3;
            player2_move = true;
        }
        else if (*text == 'k')
        {
            player2_dir = 0;
            player2_move = true;
        }
        else
        {
            player2_move = false;
        }
    }
    
    // if (c2 != NULL)
    // {
    //     //std::cout << "202" << std::endl;
    //     if (*c2 == 'i')
    //     {
    //         //std::cout << "205" << std::endl;
    //         player2_dir = 3;
    //         player2_move = true;
    //     }
    //     else if (*c2 == 'k')
    //     {
    //         player2_dir = 0;
    //         player2_move = true;
    //     }
    //      else
    //     {
    //         player2_move = false;
    //     }
    // }
    // else
    // {
    //     player2_move = false;
    // }
    
    
    
}

// void Game::handleEvents()
// {
//     while(SDL_PollEvent( &Game::event ) != 0)
//     {
//         if( Game::event.type == SDL_QUIT )
// 	    {
// 	    	Game::isRunning = false;
// 	    }

//         const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

//         if ( currentKeyStates[ SDL_SCANCODE_ESCAPE ] )
//         {
//             Game::isRunning = false;    
//         }
// 		else if( currentKeyStates[ SDL_SCANCODE_UP ] )
// 		{
//             player_dir = 3;
//             player_move = true;
//             player_VelY = -VELOCITY;
//             player_VelX = 0;
// 		}
// 		else if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
// 		{   
//             player_dir = 0;
//             player_move = true;
//             player_VelY = VELOCITY;
//             player_VelX = 0;
// 		}
// 		else if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
// 		{
//             player_dir = 1;
//             player_move = true;
//             player_VelX = -VELOCITY;
//             player_VelY = 0;
// 		}
// 		else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
// 		{   
//             player_dir = 2;
//             player_move = true;
//             player_VelX = VELOCITY;
//             player_VelY = 0;
// 		}
//         else
//         {
//             player_VelX = 0;
//             player_VelY = 0;
//             player_move = false;
//             player_frame = 0;
//         }

// 		if( currentKeyStates[ SDL_SCANCODE_W ] )
// 		{
//             player2_dir = 3;
//             player2_move = true;
//             player2_VelY = -VELOCITY;
//             player2_VelX = 0;
// 		}
// 		else if( currentKeyStates[ SDL_SCANCODE_S ] )
// 		{   
//             player2_dir = 0;
//             player2_move = true;
//             player2_VelY = VELOCITY;
//             player2_VelX = 0;
// 		}
// 		else if( currentKeyStates[ SDL_SCANCODE_A ] )
// 		{
//             player2_dir = 1;
//             player2_move = true;
//             player2_VelX = -VELOCITY;
//             player2_VelY = 0;
// 		}
// 		else if( currentKeyStates[ SDL_SCANCODE_D ] )
// 		{   
//             player2_dir = 2;
//             player2_move = true;
//             player2_VelX = VELOCITY;
//             player2_VelY = 0;
// 		}
//         else
//         {
//             player2_VelX = 0;
//             player2_VelY = 0;
//             player2_move = false;
//             player2_frame = 0;
//         }        
//     }
// }

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

    player_X += player_VelX;
    playerCollider.x += player_VelX;
    

    if( ( player_X < 0 ) || ( player_X + player_width > SCREEN_WIDTH ) || touchesWall( playerCollider, tileSet ))
    {
        player_X -= player_VelX;
        playerCollider.x -= player_VelX;
    }

    player_Y += player_VelY;
    playerCollider.y += player_VelY;

    if( ( player_Y < 0 ) || ( player_Y + player_height > SCREEN_HEIGHT ) || touchesWall( playerCollider, tileSet ))
    {
        player_Y -= player_VelY;
        playerCollider.y -= player_VelY;
    }


    player2_X += player2_VelX;
    player2_Y += player2_VelY;

    if( ( player2_X < 0 ) || ( player2_X + 64 > SCREEN_WIDTH ) )
    {
        player2_X -= player_VelX;
    }

    if( ( player2_Y < 0 ) || ( player2_Y + 64 > SCREEN_HEIGHT ) )
    {
        player2_Y -= player2_VelY;
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor( renderer, 255, 255, 200, 255 );
    SDL_RenderClear(renderer);
    
    SDL_Rect* player_currentClip = &animations[player_dir][player_frame];
    SDL_Rect* player2_currentClip = &animations[player2_dir][player2_frame];	
    
    for( int i = 0; i < TOTAL_TILES; ++i )
	{
        tileSheet.render(tileSet[i].x, tileSet[i].y, NULL, TILE_WIDTH, TILE_HEIGHT, 1);
	}

    player.render( player_X, player_Y, player_currentClip, player_width, player_height, 1);
    player2.render( player2_X, player2_Y, player2_currentClip, 32, 32, 2);
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