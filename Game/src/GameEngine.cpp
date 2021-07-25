#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include "Global_defs.hpp"
#include "GameEngine.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Collision.hpp"

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

int animation_speed = 100;
 
int VELOCITY = 2;
int VELOCITY_ENEMY = 2;

int player_frame = 0;
int player_dir = 0;
bool player_move = false;
int player_X = 64;
int player_Y = 64;
int player_VelX = 0;
int player_VelY = 0;
bool player_display = true;
SDL_Rect playerCollider;

int player2_frame = 0;
int player2_dir = 0;
bool player2_move = false;
int player2_X = 192;
int player2_Y = 64;
int player2_VelX = 0;
int player2_VelY = 0;
SDL_Rect player2Collider;

int enemy_frame = 0;
int enemy_dir = 1;
bool enemy_move = false;
int enemy_X = 256;
int enemy_Y = 64;
int enemy_VelX = VELOCITY_ENEMY;
int enemy_VelY = VELOCITY_ENEMY;
SDL_Rect enemyCollider;

int player_width = 64;
int player_height = 64;

int coin_frame = 0;

int TOTAL_TILES = 0;
int TOTAL_COINS = 0;
int TOTAL_COINS_2 = 0;
int TOTAL_GRASS = 0;


std::vector<SDL_Rect> tileSet;
std::vector<SDL_Rect> grassSet;
std::vector<SDL_Rect> coinSet;
std::vector<SDL_Rect> coin2Set;

std::vector<std::vector<SDL_Rect> > player_animations;
std::vector<std::vector<SDL_Rect> > coin_animations;
std::vector<std::vector<SDL_Rect> > enemy_animations;

TextureManager player;
TextureManager player2;
TextureManager enemy;

TextureManager tileSheet;
TextureManager coinSheet;
TextureManager coin2Sheet;
TextureManager grassSheet;

//The music that will be played
Mix_Music *gMusic = NULL;
Mix_Music *gMusic2 = NULL;

//The sound effects that will be used
Mix_Chunk *gScratch = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *coinSound = NULL;
Mix_Chunk *gLow = NULL;

bool Game::isRunning = false;

void setTiles();

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


        
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
		{
			printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		}

        isRunning = true;
    }
}

void Game::loadMedia()
{
    // Players loaded
    player2.loadFromFile("data/player.png");
    player2.setColor(0, 235, 235);
    player.loadFromFile("data/player.png");
    player.setColor(235, 235, 235);
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
        player_animations.push_back(row);
    }

    // Colliders for the players
    playerCollider.x = player_X + player_width/8;
    playerCollider.y = player_Y + player_height/8;
    playerCollider.w = 3*player_width/4;
    playerCollider.h = 3*player_height/4;

    player2Collider.x = player2_X + player_width/8;
    player2Collider.y = player2_Y + player_height/8;
    player2Collider.w = 3*player_width/4;
    player2Collider.h = 3*player_height/4;

    enemy.loadFromFile("data/enemy.png");
    // player2.setColor(0, 235, 235);

    // Enemy Animations
    for (int i = 0; i < 4; i++)
    {
        std::vector<SDL_Rect> row;
        for (int j = 0; j < 3; j++)
        {
            SDL_Rect src;
            src.x = j * 48;
            src.y = i * 64; 
            src.w = 48;
            src.h = 64;
            row.push_back(src);
        }
        enemy_animations.push_back(row);
    }

    // Colliders for the enemy
    enemyCollider.x = enemy_X + player_width/8;
    enemyCollider.y = enemy_Y + player_height/8;
    enemyCollider.w = 3*player_width/4;
    enemyCollider.h = 3*player_height/4;

    coinSheet.loadFromFile("data/coin_gold.png");
    coin2Sheet.loadFromFile("data/coin_silver.png");
    // Coin Animations
    for (int i = 0; i < 1; i++)
    {
        std::vector<SDL_Rect> row;
        for (int j = 0; j < 8; j++)
        {
            SDL_Rect src;
            src.x = j * 32;
            src.y = i * 32; 
            src.w = 32;
            src.h = 32;
            row.push_back(src);
        }
        coin_animations.push_back(row);
    }

    // Map loaded
    grassSheet.loadFromFile("data/grass.png");
    tileSheet.loadFromFile("data/wall_wood.png");
    setTiles();

    // Music and SoundEffects loaded
    gMusic = Mix_LoadMUS( "data/INGAME_MUSIC.ogg" );
	if( gMusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
	}

    // Music and SoundEffects loaded
    gMusic2 = Mix_LoadMUS( "data/INTENSE_MUSIC.ogg" );
	if( gMusic2 == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
	}

    gHigh = Mix_LoadWAV( "data/death.ogg" );
    if( gHigh == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	}

    coinSound = Mix_LoadWAV( "data/coin_sound.wav" );
    if( coinSound == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	}

    Mix_PlayMusic( gMusic, -1 );

}

// void Game::handleEvents(char* text, int ind)
// {
//     // input_struct1 inp_str_game;
//     // input_struct2 inp_str_game2;
//     // char* c1 = inp_str_game.text_p1;
//     // char* c2 = inp_str_game2.text_p2;
    
//     if (ind == 0)
//     {
//         if( Game::event.type == SDL_QUIT )
// 	    {
// 	    	Game::isRunning = false;
// 	    }

//         const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

//         // if ( currentKeyStates[ SDL_SCANCODE_SPACE] )
//         // {
//         //     if( Mix_PlayingMusic() == 0 )
// 		// 	    {
// 		// 		// Play the music
// 		// 		Mix_PlayMusic( gMusic, -1 );
// 		// 	    }
//         //     //If music is being played
// 		//     else
// 		//     {
// 		//     //If the music is paused
// 		//     if( Mix_PausedMusic() == 1 )
// 		//     {
// 		//     	//Resume the music
// 		//     	Mix_ResumeMusic();
// 		//     }
// 		//     //If the music is playing
// 		//     else
// 		//     {
// 		//     	//Pause the music
// 		//     	Mix_PauseMusic();
// 		//     }
// 		//     }
//         // }

//         if ( currentKeyStates[ SDL_SCANCODE_ESCAPE ] )
//         {
//             Game::isRunning = false;
//             Mix_HaltMusic();    
//         }
// 		else if( currentKeyStates[ SDL_SCANCODE_UP ] )
// 		{
//             player_dir = 3;
//             player_move = true;
//         }
//         else if (*text == 's')
//         {
//             player_dir = 0;
//             player_move = true;
//         }
//         else
//         {
//             player_move = false;
//         }
//     }
//     else
//     {
//         std::cout << "ind " << ind << std::endl;
//         player_move = false;
//         if (*text == 'i')
//         {
//             //std::cout << "183.Game" << std::endl;
//             player2_dir = 3;
//             player2_move = true;
//         }
//         else if (*text == 'k')
//         {
//             player2_dir = 0;
//             player2_move = true;
//         }
//         else
//         {
//             player2_move = false;
//         }
//     }
    
//     // if (c2 != NULL)
//     // {
//     //     //std::cout << "202" << std::endl;
//     //     if (*c2 == 'i')
//     //     {
//     //         //std::cout << "205" << std::endl;
//     //         player2_dir = 3;
//     //         player2_move = true;
//     //     }
//     //     else if (*c2 == 'k')
//     //     {
//     //         player2_dir = 0;
//     //         player2_move = true;
//     //     }
//     //      else
//     //     {
//     //         player2_move = false;
//     //     }
//     // }
//     // else
//     // {
//     //     player2_move = false;
//     // }
    
    
    
// }

void Game::handleEvents()
{
    while(SDL_PollEvent( &Game::event ) != 0)
    {
        if( Game::event.type == SDL_QUIT )
	    {
	    	Game::isRunning = false;
	    }

        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

        if ( currentKeyStates[ SDL_SCANCODE_ESCAPE ] )
        {
            Game::isRunning = false;    
        }
		else if( currentKeyStates[ SDL_SCANCODE_UP ] )
		{
            player_dir = 3;
            player_move = true;
            player_VelY = -VELOCITY;
            player_VelX = 0;
		}
		else if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
		{   
            player_dir = 0;
            player_move = true;
            player_VelY = VELOCITY;
            player_VelX = 0;
		}
		else if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
		{
            player_dir = 1;
            player_move = true;
            player_VelX = -VELOCITY;
            player_VelY = 0;
		}
		else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
		{   
            player_dir = 2;
            player_move = true;
            player_VelX = VELOCITY;
            player_VelY = 0;
		}
        else
        {
            player_VelX = 0;
            player_VelY = 0;
            player_move = false;
            player_frame = 0;
        }

		if( currentKeyStates[ SDL_SCANCODE_W ] )
		{
            player2_dir = 3;
            player2_move = true;
            player2_VelY = -VELOCITY;
            player2_VelX = 0;
		}
		else if( currentKeyStates[ SDL_SCANCODE_S ] )
		{   
            player2_dir = 0;
            player2_move = true;
            player2_VelY = VELOCITY;
            player2_VelX = 0;
		}
		else if( currentKeyStates[ SDL_SCANCODE_A ] )
		{
            player2_dir = 1;
            player2_move = true;
            player2_VelX = -VELOCITY;
            player2_VelY = 0;
		}
		else if( currentKeyStates[ SDL_SCANCODE_D ] )
		{   
            player2_dir = 2;
            player2_move = true;
            player2_VelX = VELOCITY;
            player2_VelY = 0;
		}
        else
        {
            player2_VelX = 0;
            player2_VelY = 0;
            player2_move = false;
            player2_frame = 0;
        }        
    }
}

int state = 0;

void Game::update()
{
    enemy_frame = (SDL_GetTicks() / animation_speed) % 3;
    
    coin_frame = (SDL_GetTicks() / animation_speed) % 4;
    
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
    

    if( ( player_X < 0 ) || ( player_X + player_width > SCREEN_WIDTH ) || touchesWall( playerCollider, tileSet, TOTAL_TILES))
    {
        player_X -= player_VelX;
        playerCollider.x -= player_VelX;
    }

    player_Y += player_VelY;
    playerCollider.y += player_VelY;

    if( ( player_Y < 0 ) || ( player_Y + player_height > SCREEN_HEIGHT ) || touchesWall( playerCollider, tileSet, TOTAL_TILES ))
    {
        player_Y -= player_VelY;
        playerCollider.y -= player_VelY;
    }

    player2_X += player2_VelX;
    player2Collider.x += player2_VelX;
    

    if( ( player2_X < 0 ) || ( player2_X + player_width > SCREEN_WIDTH ) || touchesWall( player2Collider, tileSet, TOTAL_TILES ))
    {
        player2_X -= player2_VelX;
        player2Collider.x -= player2_VelX;
    }

    player2_Y += player2_VelY;
    player2Collider.y += player2_VelY;

    if( ( player2_Y < 0 ) || ( player2_Y + player_height > SCREEN_HEIGHT ) || touchesWall( player2Collider, tileSet, TOTAL_TILES ))
    {
        player2_Y -= player2_VelY;
        player2Collider.y -= player2_VelY;
    }

    bool check_enemy_collide = false;
    
    
    enemy_X += enemy_VelX;
    enemyCollider.x += enemy_VelX;

    if( ( enemy_X < 0 ) || ( enemy_X + player_width > SCREEN_WIDTH ) || touchesWall( enemyCollider, tileSet, TOTAL_TILES))
    {
        enemy_X -= enemy_VelX;
        enemyCollider.x -= enemy_VelX;
        check_enemy_collide = true;
    }

    enemy_Y += enemy_VelY;
    enemyCollider.y += enemy_VelY;

    if( ( enemy_Y < 0 ) || ( enemy_Y + player_height > SCREEN_HEIGHT ) || touchesWall( enemyCollider, tileSet, TOTAL_TILES ))
    {
        enemy_Y -= enemy_VelY;
        enemyCollider.y -= enemy_VelY;
        check_enemy_collide = true;
    }
    
    while(check_enemy_collide)
    {
        check_enemy_collide = false;

        enemy_dir = rand() % 4;

        if( enemy_dir == 0)
	    {
            enemy_VelY = -VELOCITY_ENEMY;
            enemy_VelX = 0;
	    }
	    else if( enemy_dir == 2 )
	    {   
            enemy_VelY = VELOCITY_ENEMY;
            enemy_VelX = 0;
	    }
	    else if( enemy_dir == 3 )
	    {
            enemy_VelX = -VELOCITY_ENEMY;
            enemy_VelY = 0;
	    }
	    else if( enemy_dir == 1)
	    {   
            enemy_VelX = VELOCITY_ENEMY;
            enemy_VelY = 0;
	    }
        
        enemy_X += enemy_VelX;
        enemyCollider.x += enemy_VelX;

        if( ( enemy_X < 0 ) || ( enemy_X + player_width > SCREEN_WIDTH ) || touchesWall( enemyCollider, tileSet, TOTAL_TILES))
        {
            enemy_X -= enemy_VelX;
            enemyCollider.x -= enemy_VelX;
            check_enemy_collide = true;
        }

        enemy_Y += enemy_VelY;
        enemyCollider.y += enemy_VelY;

        if( ( enemy_Y < 0 ) || ( enemy_Y + player_height > SCREEN_HEIGHT ) || touchesWall( enemyCollider, tileSet, TOTAL_TILES ))
        {
            enemy_Y -= enemy_VelY;
            enemyCollider.y -= enemy_VelY;
            check_enemy_collide = true;
        }
    }

    if (checkCollision(playerCollider, enemyCollider))
    {
        // player_display = false;
        playerCollider.x = 64;
        playerCollider.y = 64;
        player_X = 64;
        player_Y = 64;
        // playerCollider.w = 0;
        // playerCollider.h = 0;
        Mix_PlayChannel(-1, gHigh, 0);
    }

    for( int i = 0; i < TOTAL_COINS; i++ )
    {
        //If the collision box touches the coin 
        if( checkCollision( playerCollider, coinSet[ i ] ) )
        {
            coinSet.erase(coinSet.begin() + i);
            TOTAL_COINS--;
            Mix_PlayChannel(-1, coinSound, 0);
            break;
        }
    }

    for( int i = 0; i < TOTAL_COINS_2; i++ )
    {
        //If the collision box touches the coin 
        if( checkCollision( player2Collider, coin2Set[ i ] ) )
        {
            coin2Set.erase(coin2Set.begin() + i);
            TOTAL_COINS_2--;
            Mix_PlayChannel(-1, coinSound, 0);
            break;
        }
    }

    if (TOTAL_COINS == 0 && TOTAL_COINS_2 == 0 && state == 0)
    {
        Mix_HaltMusic();
        Mix_PlayMusic(gMusic2, -1);
        VELOCITY_ENEMY++;
        state = 1;
    }   
}



void Game::render()
{
    SDL_SetRenderDrawColor( renderer, 170, 255, 170, 255 );
    SDL_RenderClear(renderer);
    
    SDL_Rect* player_currentClip = &player_animations[player_dir][player_frame];
    SDL_Rect* player2_currentClip = &player_animations[player2_dir][player2_frame];	
    SDL_Rect* enemy_currentClip = &enemy_animations[enemy_dir][enemy_frame];	

    SDL_Rect* coin_currentClip = &coin_animations[0][coin_frame];
    
    for ( int i = 0; i < TOTAL_TILES; i++ )
	{
        tileSheet.render(tileSet[i].x, tileSet[i].y, NULL, TILE_WIDTH, TILE_HEIGHT, 1);
	}

    for ( int i = 0; i < TOTAL_TILES; i++ )
	{
        grassSheet.render(grassSet[i].x, grassSet[i].y, NULL, TILE_WIDTH, TILE_HEIGHT, 1);
	}

    for ( int i = 0; i < TOTAL_COINS; i++ )
	{
        coinSheet.render(coinSet[i].x, coinSet[i].y, coin_currentClip, TILE_WIDTH, TILE_HEIGHT, 1);
	}

    for ( int i = 0; i < TOTAL_COINS_2; i++ )
	{
        coin2Sheet.render(coin2Set[i].x, coin2Set[i].y, coin_currentClip, TILE_WIDTH, TILE_HEIGHT, 1);
	}

    if (player_display)
    {
        player.render( player_X, player_Y, player_currentClip, player_width, player_height, 1);
    }
    player2.render( player2_X, player2_Y, player2_currentClip, player_width, player_height, 1);
    enemy.render( enemy_X, enemy_Y, enemy_currentClip, player_width, player_height, 1);
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    // Free loaded images
    player.free();
    player2.free();
    tileSheet.free();

    //Deallocate tiles
	for( int i = 0; i < TOTAL_TILES; ++i )
	{
        tileSet.pop_back();
	}
    
    Mix_FreeMusic( gMusic );
    Mix_FreeChunk( gScratch );
	Mix_FreeChunk( gHigh );
	Mix_FreeChunk( coinSound );
	Mix_FreeChunk( gLow );

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
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
                else if (tileType == 2)
                {
                    SDL_Rect newCoin;
                    newCoin.x = x * TILE_WIDTH;
                    newCoin.y = y * TILE_HEIGHT;
                    newCoin.w = TILE_WIDTH;
                    newCoin.h = TILE_HEIGHT;
                    coinSet.push_back(newCoin);
                    TOTAL_COINS++;
                }
                else if (tileType == 3)
                {
                    SDL_Rect newCoin;
                    newCoin.x = x * TILE_WIDTH;
                    newCoin.y = y * TILE_HEIGHT;
                    newCoin.w = TILE_WIDTH;
                    newCoin.h = TILE_HEIGHT;
                    coin2Set.push_back(newCoin);
                    TOTAL_COINS_2++;
                }

                if (tileType != 1)
                {
                    SDL_Rect newTile;
                    newTile.x = x * TILE_WIDTH;
                    newTile.y = y * TILE_HEIGHT;
                    newTile.w = TILE_WIDTH;
                    newTile.h = TILE_HEIGHT;
                    grassSet.push_back(newTile);
                    TOTAL_GRASS++;
                }
            }
		}
	}

    //Close the file
    map.close();
}