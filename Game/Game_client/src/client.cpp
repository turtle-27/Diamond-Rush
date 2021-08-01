#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <cstring>
#include <unistd.h>
#include <termios.h>
#include <vector>

#include "GameEngine.hpp"
#include "Global_networking.hpp"

#define PORT 1234

Game *game = nullptr;

using namespace std;

SDLNet_SocketSet socket_set;
int main(int argc, char** argv)
{
    const int FPS = 60;
    const int time_per_frame = 1000/FPS; 

    Uint32 frameStart;
    int frameTime;
    struct glob* glb;
    glb = Game::getStruct();
    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();

    IPaddress ip;
    SDLNet_ResolveHost(&ip, "127.0.0.1" , PORT);
    socket_set = SDLNet_AllocSocketSet(1);
    TCPsocket client = SDLNet_TCP_Open(&ip);
    SDLNet_TCP_AddSocket(socket_set, client);
    int recv_size[20] = {361, 1, 1, 3, 3, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1};
    char msg_send[1];
    int con_size = 400;
    char msg_recv[con_size];
    const char* text;

    vector<string> vec1;
    bool flag = true;
        
    while(flag)
    {       
        if(SDLNet_CheckSockets(socket_set, 0) > 0)
        {
            int  num_recv  = SDLNet_TCP_Recv(client, msg_recv, con_size);
            flag = false;
            
            if(num_recv <= 0)
            {
                cout << "Server Disconnected" << endl;
            }

            int start = 0;
            for(int j = 0; j<20; j++)
            {
                    string out = "";
                    int i;
                    for(i = start; i< start+recv_size[j]; i++)
                    {
                            out += msg_recv[i];
                    }
                    start = i;
                    vec1.push_back(out);
            }
                
        }
    }

        
    game = new Game();

    game->init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 950, false);
    
    if (vec1.size() != 0)
    {
        glb->mazefile = vec1[0];
    }
    
    game->loadMedia();


    while(game->running())
    {
        
        // Recv data
        vector<string> vec;
        if(SDLNet_CheckSockets(socket_set, 0) > 0)
        {
                int  num_recv  = SDLNet_TCP_Recv(client, msg_recv, con_size);
                if(num_recv <= 0)
                {
                    continue;
                }
                
                int start = 0;
                for(int j = 0; j<20; j++)
                {
                        string out = "";
                        int i;
                        for(i = start; i< start+recv_size[j]; i++)
                        {
                                out += msg_recv[i];
                        }
                        start = i;
                        vec.push_back(out);
                }

        }

        // Recieved data assigned to appropriate variables
        if (vec.size() != 0)
        {
            glb->mazefile = vec[0];
            glb->player_dir = stoi(vec[1]);
            glb->player_move = stoi(vec[2]);
            glb->player_X = stoi(vec[3]);
            glb->player_Y = stoi(vec[4]);
            glb->enemy1.enemy_dir = stoi(vec[5]);
            glb->enemy2.enemy_dir = stoi(vec[6]);
            glb->enemy3.enemy_dir = stoi(vec[7]);
            glb->enemy4.enemy_dir = stoi(vec[8]);
            glb->enemy1.enemy_X = stoi(vec[9]);
            glb->enemy1.enemy_Y = stoi(vec[10]);
            glb->enemy2.enemy_X = stoi(vec[11]);
            glb->enemy2.enemy_Y = stoi(vec[12]);
            glb->enemy3.enemy_X = stoi(vec[13]);
            glb->enemy3.enemy_Y = stoi(vec[14]);
            glb->enemy4.enemy_X = stoi(vec[15]);
            glb->enemy4.enemy_Y = stoi(vec[16]);
            glb->player_lifes = stoi(vec[17]);
            glb->player_power_invisble = stoi(vec[18]);
            glb->player_power_invisble_count = stoi(vec[19]);
        }


        frameStart = SDL_GetTicks();
        
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;

        if (time_per_frame > frameTime)
        {
            SDL_Delay(time_per_frame - frameTime);
        }

        game->handleEvents();  
        
        // Send data
        string send_string = "";

        string temp;
        send_string += std::to_string(glb->player2_dir);
        send_string += std::to_string(glb->player2_move);
        
        if(glb->player2_X < 10)
        {
            temp = "00";
        }
        else if (glb->player2_X < 100)
        {
            temp = "0";
        }
        else
        {
            temp = "";
        }
        send_string += temp + std::to_string(abs(glb->player2_X));
        
        if(glb->player2_Y < 10)
        {
            temp = "00";
        }
        else if (glb->player2_Y < 100)
        {
            temp = "0";
        }
        else
        {
            temp = "";
        }
        send_string += temp + std::to_string(abs(glb->player2_Y));

        send_string += std::to_string(glb->player2_lifes);

        send_string += std::to_string(glb->player2_power_invisble);

        send_string += std::to_string(glb->player2_power_invisble_count);
        
        text = send_string.c_str();

        SDLNet_TCP_Send(client, text, strlen(text));
    }

    game->clean();
    
    SDLNet_TCP_Close(client);
    
    SDLNet_Quit();
    SDL_Quit;
}