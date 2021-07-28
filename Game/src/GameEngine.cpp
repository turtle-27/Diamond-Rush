#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include "Global_defs.hpp"
#include "loadMedia.hpp"

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

        //Initialize SDL_ttf
		if( TTF_Init() == -1 )
		{
			printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		}
        
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
		{
			printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		}

        isRunning = true;
        quit = true;
    }

    srand((unsigned int)time(NULL)); //seed random number generator with system time
	initialize();      //initialize the maze
	generate();      //generate the maze
	mazefile = savebmp();
    mazefile = addCoins(mazefile, initial_coins1, initial_coins2);
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
        
        if( currentKeyStates[ SDL_SCANCODE_SPACE ] && !player_power_invisble && player_power_invisble_count > 0)
		{
            player.setAlpha(150);
            player_power_invisble = true; 
            player_invisble_time = SDL_GetTicks();
            player_power_invisble_count--;
            hud_player_power_invisble_cnt.loadFromRenderedText( std::to_string(player_power_invisble_count), {0 ,0, 0}, gFont );       
		}

        if( currentKeyStates[ SDL_SCANCODE_LSHIFT] && !player2_power_invisble && player2_power_invisble_count > 0)
		{
            player2.setAlpha(150);
            player2_power_invisble = true; 
            player2_invisble_time = SDL_GetTicks();       
            player2_power_invisble_count--;
            hud_player2_power_invisble_cnt.loadFromRenderedText( std::to_string(player2_power_invisble_count), {0 ,0, 0}, gFont );
		}

		if( currentKeyStates[ SDL_SCANCODE_UP ] )
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

// change of music 
int state = 0;

void Game::update()
{
    coin_frame = (SDL_GetTicks() / animation_speed) % 4;

    diamond_frame = (SDL_GetTicks() / animation_speed) % 7;

    enemy1.move(tileSet, wallSet);
    enemy2.move(tileSet, wallSet);
    enemy3.move(tileSet, wallSet);
    enemy4.move(tileSet, wallSet);

    if ((SDL_GetTicks() - player_invisble_time >= 3000) && player_power_invisble)
    {
        player_power_invisble = false;
        player.setAlpha(255);
    }

    if ((SDL_GetTicks() - player2_invisble_time >= 3000) && player2_power_invisble)
    {
        player2_power_invisble = false;
        player2.setAlpha(255);
    }

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
    

    if( touchesWall( playerCollider, tileSet) || touchesWall( playerCollider, wallSet) )
    {
        player_X -= player_VelX;
        playerCollider.x -= player_VelX;
    }

    player_Y += player_VelY;
    playerCollider.y += player_VelY;

    if( touchesWall( playerCollider, tileSet  ) || touchesWall( playerCollider, wallSet ) )
    {
        player_Y -= player_VelY;
        playerCollider.y -= player_VelY;
    }

    player2_X += player2_VelX;
    player2Collider.x += player2_VelX;
    

    if( touchesWall( player2Collider, tileSet ) || touchesWall( player2Collider, wallSet ) )
    {
        player2_X -= player2_VelX;
        player2Collider.x -= player2_VelX;
    }

    player2_Y += player2_VelY;
    player2Collider.y += player2_VelY;

    if( touchesWall( player2Collider, tileSet) ||  touchesWall( player2Collider, wallSet) )
    {
        player2_Y -= player2_VelY;
        player2Collider.y -= player2_VelY;
    }
    
    for (int i = 0; i < 4; i++)
    {
        if (checkCollision(playerCollider, *enemies[i]) && !player_power_invisble 
            && player_X != 50 && player_Y != 50)
        {
            player_X = 50;
            player_Y = 50;
            playerCollider.x = player_X + player_width/8;
            playerCollider.y = player_Y + player_height/8;
            player_lifes--;
            hud_player_life.loadFromRenderedText( std::to_string(player_lifes), {0 ,0, 0}, gFont );
            Mix_PlayChannel(-1, death, 0);
        }

        if (checkCollision(player2Collider, *enemies[i]) && !player2_power_invisble 
            && player2_X != 50 && player2_Y != 850)
        {
            player2_X = 50;
            player2_Y = 850;
            player2Collider.x = player2_X + player_width/8;
            player2Collider.y = player2_Y + player_height/8;
            player2_lifes--;
            hud_player2_life.loadFromRenderedText( std::to_string(player2_lifes), {0 ,0, 0}, gFont );
            Mix_PlayChannel(-1, death, 0);
        }
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
        Mix_PlayMusic(intenseMusic, -1);
        wallSet.erase(wallSet.begin() + gate_location);
        TOTAL_WALL--;
        enemy1.ENEMY_VELOCITY++;
        enemy2.ENEMY_VELOCITY++;
        enemy3.ENEMY_VELOCITY++;
        enemy4.ENEMY_VELOCITY++;
        state = 1;
    }  

    if (checkCollision(playerCollider, diamondCollider))
    {
        diamondCollider.x = -1;
        diamondCollider.y = -1;
        diamondCollider.w = 0;
        diamondCollider.h = 0;
        diamond_display = false;
        Mix_HaltMusic();
        Mix_PlayChannel(-1, game_win, 0);
        SDL_Delay(3000);
        quit = false;
    }

    if (checkCollision(player2Collider, diamondCollider))
    {
        diamondCollider.x = -1;
        diamondCollider.y = -1;
        diamondCollider.w = 0;
        diamondCollider.h = 0;
        diamond_display = false;
        Mix_HaltMusic();
        Mix_PlayChannel(-1, game_win, 0);
        SDL_Delay(3000);
        quit = true;
    }
     
}

void Game::render()
{
    SDL_SetRenderDrawColor( renderer, 170, 255, 170, 255 );
    SDL_RenderClear(renderer);
    
    SDL_Rect* player_currentClip = &player_animations[player_dir][player_frame];
    SDL_Rect* player2_currentClip = &player_animations[player2_dir][player2_frame];	

    SDL_Rect* coin_currentClip = &coin_animations[0][coin_frame];
    SDL_Rect* diamond_currentClip = &diamond_animations[0][diamond_frame];
    
    for ( int i = 0; i < TOTAL_GRASS; i++ )
	{
        grassSheet.render(grassSet[i].x, grassSet[i].y, NULL, TILE_WIDTH, TILE_HEIGHT, 1);
	}
    
    for ( int i = 0; i < TOTAL_TILES; i++ )
	{
        tileSheet.render(tileSet[i].x, tileSet[i].y, NULL, TILE_WIDTH, TILE_HEIGHT, 1);
	}

    for ( int i = 0; i < TOTAL_WALL; i++ )
	{
        wallSheet.render(wallSet[i].x, wallSet[i].y, NULL, TILE_WIDTH, TILE_HEIGHT, 1);
	}

    for ( int i = 0; i < TOTAL_COINS; i++ )
	{
        coinSheet.render(coinSet[i].x, coinSet[i].y, coin_currentClip, TILE_WIDTH, TILE_HEIGHT, 1);
	}

    for ( int i = 0; i < TOTAL_COINS_2; i++ )
	{
        coin2Sheet.render(coin2Set[i].x, coin2Set[i].y, coin_currentClip, TILE_WIDTH, TILE_HEIGHT, 1);
	}

    if (diamond_display)
    {
        diamond.render( (MazeSizeX/2)*TILE_WIDTH, (MazeSizeY/2)*TILE_HEIGHT, diamond_currentClip, player_width, player_height, 1);
    }

    if (player_display)
    {
        player.render( player_X, player_Y, player_currentClip, player_width, player_height, 1);
    }

    if (player2_display)
    {
        player2.render( player2_X, player2_Y, player2_currentClip, player_width, player_height, 1);
    }

    enemy1.render();
    enemy2.render();
    enemy3.render();
    enemy4.render();

    SDL_Rect* hud_player_currentClip = &player_animations[0][0];

    hud_lifes.render(980, 150, NULL, 200, 50, 1);
    
    hud_player.render(980, 200, hud_player_currentClip, 50, 50, 1);
    hud_X.render(1040,200, NULL, 50, 50, 1);
    hud_player_life.render(1100, 200, NULL, 40, 50, 1);
    
    hud_player2.render(980, 300, hud_player_currentClip, 50, 50, 1);
    hud_X.render(1040,300, NULL, 50, 50, 1);
    hud_player2_life.render(1100, 300, NULL, 40, 50, 1);
    
    hud_powerUP.render(980, 400, NULL, 200, 50, 1);

    hud_player.setAlpha(150);
    hud_player.render(980, 450, hud_player_currentClip, 50, 50, 1);
    hud_player.setAlpha(255);
    hud_X.render(1040,450, NULL, 50, 50, 1);
    hud_player_power_invisble_cnt.render(1100, 450, NULL, 40, 50, 1);
    
    hud_player2.setAlpha(150);
    hud_player2.render(980, 550, hud_player_currentClip, 50, 50, 1);
    hud_player2.setAlpha(255);
    hud_X.render(1040, 550, NULL, 50, 50, 1);
    hud_player2_power_invisble_cnt.render(1100, 550, NULL, 40, 50, 1);

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    // Free loaded images
    player.free();
    player2.free();
    tileSheet.free();
    grassSheet.free();
    coinSheet.free();
    coin2Sheet.free();
    wallSheet.free();
    diamond.free();
    
    enemy1.enemy_tex.free();
    enemy2.enemy_tex.free();
    enemy3.enemy_tex.free();
    enemy4.enemy_tex.free();

    //Deallocate tiles
	for( int i = 0; i < TOTAL_TILES; ++i )
	{
        tileSet.pop_back();
	}

    //Deallocate grass
	for( int i = 0; i < TOTAL_GRASS; ++i )
	{
        grassSet.pop_back();
	}

    //Deallocate Walls
	for( int i = 0; i < TOTAL_WALL; ++i )
	{
        wallSet.pop_back();
	}

    //Deallocate Coins
	for( int i = 0; i < TOTAL_COINS; ++i )
	{
        coinSet.pop_back();
	}

     //Deallocate Coins
	for( int i = 0; i < TOTAL_COINS_2; ++i )
	{
        coin2Set.pop_back();
	}

    // Free Font
    TTF_CloseFont( gFont );
    gFont = NULL;

    // Free Music
    Mix_FreeMusic( introMusic );
    Mix_FreeMusic( ingameMusic );
    Mix_FreeMusic( intenseMusic );
	Mix_FreeChunk( coinSound );
	Mix_FreeChunk( game_win );
    Mix_FreeChunk( death );
    introMusic = NULL;
    ingameMusic = NULL;
    intenseMusic = NULL;
    coinSound = NULL;
    game_win = NULL;
    death = NULL;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    window = NULL;
	renderer = NULL;

    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void setTiles(std::string map)
{
    //Initialize the tiles
	for( int x = 0; x < MazeSizeX; x++ )
	{
		for ( int y = 0; y < MazeSizeY; y++ )
        {
            //Determines what kind of tile will be made
		    int tileType = -1;

            tileType = map[x*MazeSizeX + y] - '0';

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
            else if (tileType == 2 || tileType == 5)
            {
                SDL_Rect newWall;
                newWall.x = x * TILE_WIDTH;
                newWall.y = y * TILE_HEIGHT;
                newWall.w = TILE_WIDTH;
                newWall.h = TILE_HEIGHT;
                wallSet.push_back(newWall);
                TOTAL_WALL++;

                if (tileType == 5)
                {
                    gate_location = TOTAL_WALL-1;
                }
            }
            else if (tileType == 3)
            {
                SDL_Rect newCoin;
                newCoin.x = x * TILE_WIDTH;
                newCoin.y = y * TILE_HEIGHT;
                newCoin.w = TILE_WIDTH;
                newCoin.h = TILE_HEIGHT;
                coinSet.push_back(newCoin);
                TOTAL_COINS++;
            }
            else if (tileType == 4)
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

            if (tileType == 6)
            {
                enemy_spawn.push_back(std::make_pair(x * TILE_WIDTH,y * TILE_HEIGHT));
            }

        }
	
    }
}