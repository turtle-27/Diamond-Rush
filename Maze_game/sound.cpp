#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <stdio.h>
#include <string>


class Sound
{
    Mix_Music *music_1 = NULL;
    Mix_Chunk *alarm = NULL;
    Mix_Chunk *start = NULL;
    Mix_Chunk *death = NULL;
    Mix_Chunk *coin = NULL;
    Mix_Chunk *diamond = NULL;
    Mix_Chunk *finish = NULL;

    public:
        Sound()
        {
            if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
            {
                printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
            }

            music_1 = Mix_LoadMUS("music1.wav");
            if( music_1 == NULL )
            {
                printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
            }
            alarm = Mix_LoadWAV("alarm.wav");
            if( alarm == NULL )
            {
                printf( "Failed to load effect! SDL_mixer Error: %s\n", Mix_GetError() );
            }
            start = Mix_LoadWAV("alarm.wav");
            if( start == NULL )
            {
                printf( "Failed to load effect! SDL_mixer Error: %s\n", Mix_GetError() );
            }
            death = Mix_LoadWAV("alarm.wav");
            if( death == NULL )
            {
                printf( "Failed to load effect! SDL_mixer Error: %s\n", Mix_GetError() );
            }
            coin = Mix_LoadWAV("alarm.wav");
            if( coin == NULL )
            {
                printf( "Failed to load effect! SDL_mixer Error: %s\n", Mix_GetError() );
            }
            diamond = Mix_LoadWAV("alarm.wav");
            if( diamond == NULL )
            {
                printf( "Failed to load effect! SDL_mixer Error: %s\n", Mix_GetError() );
            }
            finish = Mix_LoadWAV("alarm.wav");
            if( finish == NULL )
            {
                printf( "Failed to load effect! SDL_mixer Error: %s\n", Mix_GetError() );
            } 
        }

        Mix_Music *get_music()
        {
            return music_1;
        }
        Mix_Chunk *get_effect(int type)
        {
            switch (type)
            {
            case 0:
                /* code */
                return alarm;
            case 1:
                return start;
            case 2:
                return death;
            case 3:
                return coin;
            case 4:
                return diamond;
            case 5:
                return finish;
            default:
                printf("Type of sound not matched");
                return NULL;
            }
        }
        ~Sound()
        {
            Mix_HaltMusic();
            Mix_HaltChannel(-1);
            Mix_FreeMusic(music_1);
            Mix_FreeChunk(alarm);
            Mix_FreeChunk(start);
            Mix_FreeChunk(death);
            Mix_FreeChunk(coin);
            Mix_FreeChunk(diamond);
            Mix_FreeChunk(finish);
            Mix_CloseAudio();
        }
};


