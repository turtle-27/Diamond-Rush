#pragma once

#include <string>

void Game::loadMedia()
{
    // Map loaded
    grassSheet.loadFromFile("data/grass.png");
    tileSheet.loadFromFile("data/wall_wood.png");
    wallSheet.loadFromFile("data/wall.png");
<<<<<<< HEAD
    
    srand((unsigned int)time(NULL)); //seed random number generator with system time
	initialize();      //initialize the maze
	generate();      //generate the maze
	mazefile = savebmp();
    mazefile = addCoins(mazefile, initial_coins1, initial_coins2);
    setTiles(mazefile);
    
=======
    setTiles(mazefile);
    
    
    
>>>>>>> 1cff642d90a013126609ab76826fd752a683e2b7
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
<<<<<<< HEAD
        
    enemy1.load("data/ghost_sprite3.png", enemy_spawn[0].first, enemy_spawn[0].second, VELOCITY);
    enemy2.load("data/ghost_sprite3.png", enemy_spawn[1].first, enemy_spawn[1].second, VELOCITY);
    enemy3.load("data/ghost_sprite3.png", enemy_spawn[2].first, enemy_spawn[2].second, VELOCITY);
    enemy4.load("data/ghost_sprite3.png", enemy_spawn[3].first, enemy_spawn[3].second, VELOCITY);
=======

    enemy1.load("data/ghost_sprite3.png", enemy_spawn[0].first, enemy_spawn[0].second);

    enemy2.load("data/ghost_sprite3.png", enemy_spawn[1].first, enemy_spawn[1].second);

    enemy3.load("data/ghost_sprite3.png", enemy_spawn[2].first, enemy_spawn[2].second);
    
    enemy4.load("data/ghost_sprite3.png", enemy_spawn[3].first, enemy_spawn[3].second);
>>>>>>> 1cff642d90a013126609ab76826fd752a683e2b7


    enemies.push_back(&enemy1.enemyCollider);
    enemies.push_back(&enemy2.enemyCollider);
    enemies.push_back(&enemy3.enemyCollider);
    enemies.push_back(&enemy4.enemyCollider);

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

    diamond.loadFromFile("data/diamond1.png");
    diamond.setColor(255, 0, 0);
    //Diamond Animation
    for (int i = 0; i < 1; i++)
    {
        std::vector<SDL_Rect> row;
        for (int j = 0; j < 7; j++)
        {
            SDL_Rect src;
            src.x = j * 30;
            src.y = i * 30; 
            src.w = 30;
            src.h = 30;
            row.push_back(src);
        }
        diamond_animations.push_back(row);
    }

    // Colliders for the diamond
    diamondCollider.x = (MazeSizeX/2)*TILE_WIDTH + player_width/8;
    diamondCollider.y = (MazeSizeY/2)*TILE_HEIGHT + player_height/8;
    diamondCollider.w = 3*player_width/4;
    diamondCollider.h = 3*player_height/4;

    //Font loading
    gFont = TTF_OpenFont( "data/OpenSans-Semibold.ttf", 100 );
    
    //Load HUD Components
	SDL_Color textColor = { 0, 0, 0 };

<<<<<<< HEAD
    hud_lifes.loadFromRenderedText( "Remaining Lives", textColor, gFont);
=======
    hud_lifes.loadFromRenderedText( "Remaining Lives", textColor, gFont );
>>>>>>> 1cff642d90a013126609ab76826fd752a683e2b7

    hud_powerUP.loadFromRenderedText( "Invisible Power", textColor, gFont );

	hud_X.loadFromRenderedText( "X", textColor, gFont );        
    
    hud_player_life.loadFromRenderedText( std::to_string(player_lifes), textColor, gFont );
    
    hud_player2_life.loadFromRenderedText( std::to_string(player2_lifes), textColor, gFont );
    
    hud_player_power_invisble_cnt.loadFromRenderedText( std::to_string(player_power_invisble_count), textColor, gFont );
    
    hud_player2_power_invisble_cnt.loadFromRenderedText( std::to_string(player2_power_invisble_count), textColor, gFont );
		
     
    hud_player.loadFromFile("data/player.png");
    hud_player.setColor(235, 235, 235);
    hud_player2.loadFromFile("data/player.png");
    hud_player2.setColor(0, 235, 235);

<<<<<<< HEAD
    menuTitle.loadFromRenderedText("DIAMOND RUSH", textColor, gFont);
    menuImage.loadFromFile("data/MenuDiamond.png");
    menuBG.loadFromFile("data/bg.jpeg");
    menuPointer.loadFromFile("data/menuPointer.jpg");

    menu1P.loadFromRenderedText("Single Player", textColor, gFont);
    menu2P.loadFromRenderedText("Two Player", textColor, gFont);
    menuExit.loadFromRenderedText("Exit", textColor, gFont);

    loseText.loadFromRenderedText("YOU LOST!!", textColor, gFont);
    winText.loadFromRenderedText("BRAVO!! YOU FOUND THE RED DIAMOND!!", {255, 100, 50}, gFont);

    lose.loadFromFile("data/GameOver.jpg");
    win.loadFromFile("data/WIN.jpeg");

=======
>>>>>>> 1cff642d90a013126609ab76826fd752a683e2b7
    // Music and SoundEffects loaded
    ingameMusic = Mix_LoadMUS( "data/INGAME_MUSIC.ogg" );
	
    intenseMusic = Mix_LoadMUS( "data/INTENSE_MUSIC.ogg" );

<<<<<<< HEAD
    introMusic = Mix_LoadMUS( "data/INTRO_MUSIC.ogg" );

=======
>>>>>>> 1cff642d90a013126609ab76826fd752a683e2b7
    death = Mix_LoadWAV( "data/death.wav" );
    
    game_win = Mix_LoadWAV( "data/VICTORY_MUSIC.ogg" );

<<<<<<< HEAD
    game_lose = Mix_LoadWAV( "data/gameOver.wav" );

    coinSound = Mix_LoadWAV( "data/coin_sound.wav" );
=======
    coinSound = Mix_LoadWAV( "data/coin_sound.wav" );

    Mix_PlayMusic( ingameMusic, -1 );

>>>>>>> 1cff642d90a013126609ab76826fd752a683e2b7
}
