#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include "Global_defs.hpp"
#include "Global_networking.hpp"
#include "loadMedia.hpp"


SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

bool Game::isRunning = false;

struct glob glb;

Game::Game()
{}
Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = SDL_WINDOW_RESIZABLE;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        SDL_SetWindowSize(window, width, height);
        renderer = SDL_CreateRenderer(window, - 1,  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
        SDL_RenderSetLogicalSize(renderer, width, height);
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
    }
}

// change of music 
int music_change = -1;

// Indicates first time change of game stage
int intenseModeFirst = 0;

// Indicates time spent in state_3 i.e end menu
int state_3_time = 0;

void Game::handleEvents()
{
    if (game_state == 0)
    {
        while(SDL_PollEvent( &Game::event ) != 0)
        {
            if( Game::event.type == SDL_QUIT )
            {
                Game::isRunning = false;
            }

            const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

            if( currentKeyStates[ SDL_SCANCODE_UP ] )
            {
                menuItem--; 
                menuItem = (3 + menuItem) % 3;
            }
            else if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
            {
                menuItem++;
                menuItem = menuItem % 3;
            } 

            if ( currentKeyStates[ SDL_SCANCODE_KP_ENTER ] )
            {
                if ( menuItem == 1)
                {
                    game_mode = 2;
                    game_state = 1;
                }
                else if (menuItem == 2)
                {   
                    Game::isRunning = false;
                    break;
                }
            }
        }
    }
    else if (game_state == 1)
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
                game_state = 3;
                break;
            }   

            if (game_mode == 2)
            {

                if( currentKeyStates[ SDL_SCANCODE_LSHIFT] && !glb.player2_power_invisble && glb.player2_power_invisble_count > 0)
                {
                    player2.setAlpha(150);
                    glb.player2_power_invisble = true; 
                    player2_invisble_time = SDL_GetTicks();       
                    glb.player2_power_invisble_count--;
                    hud_player2_power_invisble_cnt.loadFromRenderedText( std::to_string(glb.player2_power_invisble_count), {0 ,0, 0}, gFont );
                }

                if( currentKeyStates[ SDL_SCANCODE_W ] )
                {
                    glb.player2_dir = 3;
                    glb.player2_move = true;
                    player2_VelY = -VELOCITY;
                    player2_VelX = 0;
                }
                else if( currentKeyStates[ SDL_SCANCODE_S ] )
                {   
                    glb.player2_dir = 0;
                    glb.player2_move = true;
                    player2_VelY = VELOCITY;
                    player2_VelX = 0;
                }
                else if( currentKeyStates[ SDL_SCANCODE_A ] )
                {
                    glb.player2_dir = 1;
                    glb.player2_move = true;
                    player2_VelX = -VELOCITY;
                    player2_VelY = 0;
                }
                else if( currentKeyStates[ SDL_SCANCODE_D ] )
                {   
                    glb.player2_dir = 2;
                    glb.player2_move = true;
                    player2_VelX = VELOCITY;
                    player2_VelY = 0;
                }
                else
                {
                    player2_VelX = 0;
                    player2_VelY = 0;
                    glb.player2_move = false;
                    player2_frame = 0;
                }        
            }
        }
    }
}

void Game::update()
{
    if (game_state == 0)
    {
        if (music_change == -1)
        {
            Mix_FadeOutMusic(300);
            Mix_PlayMusic( introMusic, -1 );
            music_change = 0;
        }
    }
    else if (game_state == 1)
    {
        if (music_change == 0)
        {
            Mix_FadeOutMusic(300);
            Mix_PlayMusic( ingameMusic, -1 );
            music_change = 1;

            setTiles(glb.mazefile);

            // Colliders for the diamond
            diamondCollider.x = (MazeSizeX/2)*TILE_WIDTH + player_width/8;
            diamondCollider.y = (MazeSizeY/2)*TILE_HEIGHT + player_height/8;
            diamondCollider.w = 3*player_width/4;
            diamondCollider.h = 3*player_height/4;

            glb.enemy1.load("data/ghost_sprite3.png", enemy_spawn[0].first, enemy_spawn[0].second, VELOCITY);

            glb.enemy2.load("data/ghost_sprite3.png", enemy_spawn[1].first, enemy_spawn[1].second, VELOCITY);

            glb.enemy3.load("data/ghost_sprite3.png", enemy_spawn[2].first, enemy_spawn[2].second, VELOCITY);

            glb.enemy4.load("data/ghost_sprite3.png", enemy_spawn[3].first, enemy_spawn[3].second, VELOCITY);


            enemies.push_back(&glb.enemy1.enemyCollider);
            enemies.push_back(&glb.enemy2.enemyCollider);
            enemies.push_back(&glb.enemy3.enemyCollider);
            enemies.push_back(&glb.enemy4.enemyCollider);
        }
    
        coin_frame = (SDL_GetTicks() / animation_speed) % 4;

        diamond_frame = (SDL_GetTicks() / animation_speed) % 7;

        glb.enemy1.enemyCollider.x = glb.enemy1.enemy_X + (glb.enemy1.ENEMY_WIDTH)/8;
        glb.enemy1.enemyCollider.y = glb.enemy1.enemy_Y + (glb.enemy1.ENEMY_HEIGHT)/8;

        glb.enemy2.enemyCollider.x = glb.enemy2.enemy_X + (glb.enemy2.ENEMY_WIDTH)/8;
        glb.enemy2.enemyCollider.y = glb.enemy2.enemy_Y + (glb.enemy2.ENEMY_HEIGHT)/8;

        glb.enemy3.enemyCollider.x = glb.enemy3.enemy_X + (glb.enemy3.ENEMY_WIDTH)/8;
        glb.enemy3.enemyCollider.y = glb.enemy3.enemy_Y + (glb.enemy3.ENEMY_HEIGHT)/8;

        glb.enemy4.enemyCollider.x = glb.enemy4.enemy_X + (glb.enemy4.ENEMY_WIDTH)/8;
        glb.enemy4.enemyCollider.y = glb.enemy4.enemy_Y + (glb.enemy4.ENEMY_HEIGHT)/8;
        
        if (glb.player_power_invisble)
        {
            player.setAlpha(150);       
        }
        else
        {
            player.setAlpha(255);
        }

        if (game_mode == 2)
        {
            if ((SDL_GetTicks() - player2_invisble_time >= 3000) && glb.player2_power_invisble)
            {
                glb.player2_power_invisble = false;
                player2.setAlpha(255);
            }
        }
        

        if (glb.player_move)
        {
            player_frame = (SDL_GetTicks() / animation_speed) % 4;
        }

        if (game_mode == 2)
        {   
            if (glb.player2_move)
            {
                player2_frame = (SDL_GetTicks() / animation_speed) % 4;
            }
        }

        playerCollider.x = glb.player_X + player_width/8;
        playerCollider.y = glb.player_Y + player_height/8;

        
        if (game_mode == 2)
        {
            glb.player2_X += player2_VelX;
            player2Collider.x += player2_VelX;
            
            if( touchesWall( player2Collider, tileSet ) || touchesWall( player2Collider, wallSet ) )
            {
                glb.player2_X -= player2_VelX;
                player2Collider.x -= player2_VelX;
            }

            glb.player2_Y += player2_VelY;
            player2Collider.y += player2_VelY;

            if( touchesWall( player2Collider, tileSet) ||  touchesWall( player2Collider, wallSet) )
            {
                glb.player2_Y -= player2_VelY;
                player2Collider.y -= player2_VelY;
            }
        }
        
        for (int i = 0; i < 4; i++)
        {
            if (game_mode == 2)
            {
                if (checkCollision(player2Collider, *enemies[i]) && !glb.player2_power_invisble 
                    && !(glb.player2_X == 50 && glb.player2_Y == 850))
                {
                    glb.player2_X = 50;
                    glb.player2_Y = 850;
                    player2Collider.x = glb.player2_X + player_width/8;
                    player2Collider.y = glb.player2_Y + player_height/8;
                    glb.player2_lifes--;
                    hud_player2_life.loadFromRenderedText( std::to_string(glb.player2_lifes), {0 ,0, 0}, gFont );
                    Mix_PlayChannel(-1, death, 0);
                }
            }
            
        }

        if (glb.player_lifes == 0 || (glb.player2_lifes == 0 && game_mode == 2))
        {
            winner = 0;
            Mix_HaltMusic();
            Mix_PlayChannel(-1, game_lose, 0);
            state_3_time = SDL_GetTicks();
            game_state = 3;
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

        if (game_mode == 2)
        {
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
        }
        
        if (TOTAL_COINS == 0 && (TOTAL_COINS_2 == 0 || game_mode != 2) && intenseModeFirst == 0)
        {
            Mix_HaltMusic();
            Mix_PlayMusic(intenseMusic, -1);
            wallSet.erase(wallSet.begin() + gate_location);
            TOTAL_WALL--;
            glb.enemy1.ENEMY_VELOCITY++;
            glb.enemy2.ENEMY_VELOCITY++;
            glb.enemy3.ENEMY_VELOCITY++;
            glb.enemy4.ENEMY_VELOCITY++;
            intenseModeFirst = 1;
        }  

        if (checkCollision(playerCollider, diamondCollider))
        {
            diamondCollider.x = -1;
            diamondCollider.y = -1;
            diamondCollider.w = 0;
            diamondCollider.h = 0;
            diamond_display = false;
            winner = 1;
            Mix_HaltMusic();
            Mix_PlayChannel(-1, game_win, 0);
            state_3_time = SDL_GetTicks();
            game_state = 3;
        }

        if (game_mode == 2)
        {
            if (checkCollision(player2Collider, diamondCollider))
            {
                diamondCollider.x = -1;
                diamondCollider.y = -1;
                diamondCollider.w = 0;
                diamondCollider.h = 0;
                diamond_display = false;
                winner = 1;
                Mix_HaltMusic();
                Mix_PlayChannel(-1, game_win, 0);
                state_3_time = SDL_GetTicks();
                game_state = 3;
            }
        }
    }
    else if (game_state == 3)
    {
        int etime;

        if (winner == -1)
        {
            etime = 0;
        }
        else
        {
            etime = 7500;
        }
        
        if (SDL_GetTicks() - state_3_time >= etime)
        {
            tileSet.clear();
            wallSet.clear();
            grassSet.clear();
            coinSet.clear();
            coin2Set.clear();
            enemies.clear();
            enemy_spawn.clear();

            player.setAlpha(255);
            player2.setAlpha(255);

            player_frame = 0;
            glb.player_dir = 0;
            glb.player_move = false;
            glb.player_X = 50;
            glb.player_Y = 50;
            player_VelX = 0;
            player_VelY = 0;
            player_display = true;
            glb.player_power_invisble = false;
            glb.player_power_invisble_count = 3;
            player_invisble_time = 0;
            glb.player_lifes = 3;

            player2_frame = 0;
            glb.player2_dir = 0;
            glb.player2_move = false;
            glb.player2_X = 50;
            glb.player2_Y = 850;
            player2_VelX = 0;
            player2_VelY = 0;
            player2_display = true;
            glb.player2_power_invisble = false;
            glb.player2_power_invisble_count = 3;
            player2_invisble_time = 0;
            glb.player2_lifes = 3;

            coin_frame = 0;
            diamond_frame = 0;
            diamond_display = true;

            initial_coins1 = INITIAL_COINS;
            initial_coins2 = INITIAL_COINS;

            TOTAL_TILES = 0;
            TOTAL_WALL = 0;
            TOTAL_COINS = 0;
            TOTAL_COINS_2 = 0;
            TOTAL_GRASS = 0;
            intenseModeFirst = 0;
            gate_location = 0;

            game_mode = 0;
            winner = -1;

            music_change = -1;


            playerCollider.x = glb.player_X + player_width/8;
            playerCollider.y = glb.player_Y + player_height/8;
            playerCollider.w = 3*player_width/4;
            playerCollider.h = 3*player_height/4;

            player2Collider.x = glb.player2_X + player_width/8;
            player2Collider.y = glb.player2_Y + player_height/8;
            player2Collider.w = 3*player_width/4;
            player2Collider.h = 3*player_height/4;

            SDL_Color textColor = { 0, 0, 0 };

            hud_player_life.loadFromRenderedText( std::to_string(glb.player_lifes), textColor, gFont );
            hud_player2_life.loadFromRenderedText( std::to_string(glb.player2_lifes), textColor, gFont );
            hud_player_power_invisble_cnt.loadFromRenderedText( std::to_string(glb.player_power_invisble_count), textColor, gFont );
            hud_player2_power_invisble_cnt.loadFromRenderedText( std::to_string(glb.player2_power_invisble_count), textColor, gFont );
                
            game_state = 0;
        }
    } 
}

void Game::render()
{
    
    if (game_state == 0)
    {
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 0 );
        SDL_RenderClear(renderer);
	
        menuBG.render(0, 0, NULL, 1200, 950, 1);
        menuImage.render((SCREEN_WIDTH - 300)/2, (SCREEN_HEIGHT - 200)/2, NULL, 300, 200, 1);
        menuTitle.render((SCREEN_WIDTH - 600)/2, (SCREEN_HEIGHT - 200)/2 - 200, NULL, 600, 200, 1);
        menu1P.render((SCREEN_WIDTH - 200)/2, (SCREEN_HEIGHT - 200)/2 + 250, NULL, 200, 50, 1);
        menu2P.render((SCREEN_WIDTH - 200)/2, (SCREEN_HEIGHT - 200)/2 + 320, NULL, 200, 50, 1);
        menuExit.render((SCREEN_WIDTH - 80)/2 -10, (SCREEN_HEIGHT - 200)/2 + 390, NULL, 80, 50, 1);

        if (menuItem == 0)
        {
            menuPointer.render((SCREEN_WIDTH - 200)/2 - 50, (SCREEN_HEIGHT - 200)/2 + 250, NULL, 50, 50, 1);
        }
        else if (menuItem == 1)
        {
            menuPointer.render((SCREEN_WIDTH - 200)/2 - 50, (SCREEN_HEIGHT - 200)/2 + 320, NULL, 50, 50, 1);
        }
        else if (menuItem == 2)
        {
            menuPointer.render((SCREEN_WIDTH - 80)/2 - 60, (SCREEN_HEIGHT - 200)/2 + 390, NULL, 50, 50, 1);
        }
    }
    else if (game_state == 1)
    { 
        SDL_SetRenderDrawColor( renderer, 170, 255, 170, 255 );
        SDL_RenderClear(renderer);

        SDL_Rect* player_currentClip = &player_animations[glb.player_dir][player_frame];
        SDL_Rect* player2_currentClip = &player_animations[glb.player2_dir][player2_frame];	

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

        if (game_mode == 2)
        {
            for ( int i = 0; i < TOTAL_COINS_2; i++ )
            {
                coin2Sheet.render(coin2Set[i].x, coin2Set[i].y, coin_currentClip, TILE_WIDTH, TILE_HEIGHT, 1);
            }
        }

        if (diamond_display)
        {
            diamond.render( (MazeSizeX/2)*TILE_WIDTH, (MazeSizeY/2)*TILE_HEIGHT, diamond_currentClip, player_width, player_height, 1);
        }

        if (player_display)
        {
            player.render( glb.player_X, glb.player_Y, player_currentClip, player_width, player_height, 1);
        }

        if (game_mode == 2)
        {
            if (player2_display)
            {
                player2.render( glb.player2_X, glb.player2_Y, player2_currentClip, player_width, player_height, 1);
            }
        }

        glb.enemy1.render();
        glb.enemy2.render();
        glb.enemy3.render();
        glb.enemy4.render();

        SDL_Rect* hud_player_currentClip = &player_animations[0][0];

        hud_lifes.render(980, 150, NULL, 200, 50, 1);
        
        hud_player.render(980, 200, hud_player_currentClip, 50, 50, 1);
        hud_X.render(1040,200, NULL, 50, 50, 1);
        hud_player_life.loadFromRenderedText( std::to_string(glb.player_lifes), {0 ,0, 0}, gFont );
        hud_player_life.render(1100, 200, NULL, 40, 50, 1);
        
        
        if (game_mode == 2)
        {
            hud_player2.render(980, 300, hud_player_currentClip, 50, 50, 1);
            hud_X.render(1040,300, NULL, 50, 50, 1);
            hud_player2_life.render(1100, 300, NULL, 40, 50, 1);
        }
        
        hud_powerUP.render(980, 400, NULL, 200, 50, 1);

        hud_player.setAlpha(150);
        hud_player.render(980, 450, hud_player_currentClip, 50, 50, 1);
        hud_player.setAlpha(255);
        hud_X.render(1040,450, NULL, 50, 50, 1);
        hud_player_power_invisble_cnt.loadFromRenderedText( std::to_string(glb.player_power_invisble_count), {0 ,0, 0}, gFont );
        hud_player_power_invisble_cnt.render(1100, 450, NULL, 40, 50, 1);
        
        if (game_mode == 2)
        {
            hud_player2.setAlpha(150);
            hud_player2.render(980, 550, hud_player_currentClip, 50, 50, 1);
            hud_player2.setAlpha(255);
            hud_X.render(1040, 550, NULL, 50, 50, 1);
            hud_player2_power_invisble_cnt.render(1100, 550, NULL, 40, 50, 1);
        }
    }
    else if (game_state == 3)
    {
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
        SDL_RenderClear(renderer);

        menuBG.render(0, 0, NULL, 1200, 950, 1);

        if (winner == 0)
        {
            lose.render((SCREEN_WIDTH - 600)/2, (SCREEN_HEIGHT - 600)/4, NULL, 600, 600, 1);
        }
        else if (winner == 1)
        {
            win.render((SCREEN_WIDTH - 600)/2, (SCREEN_HEIGHT - 600)/4, NULL, 600, 600, 1);
            winText.render((SCREEN_WIDTH - 800)/2, (SCREEN_HEIGHT - 100)/2 + 300, NULL, 800, 100, 1);
        }
    }

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

    hud_player_life.free();
    hud_player2_life.free();
    hud_player_power_invisble_cnt.free();
    hud_player2_power_invisble_cnt.free();
    hud_X.free();
    hud_player.free();
    hud_player2.free();
    hud_lifes.free();
    hud_powerUP.free();
    
    glb.enemy1.enemy_tex.free();
    glb.enemy2.enemy_tex.free();
    glb.enemy3.enemy_tex.free();
    glb.enemy4.enemy_tex.free();

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

struct glob* Game::getStruct()
{
    return &glb;
}