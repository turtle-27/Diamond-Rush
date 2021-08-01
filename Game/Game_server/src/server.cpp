#include "GameEngine.hpp"
#include "Global_networking.hpp"
#include <SDL2/SDL_net.h>

#define PORT 1234
#define MAX_SOCKETS 1


TCPsocket server;
TCPsocket client[1];
SDLNet_SocketSet socket_set;
int next_ind = 0;
int AcceptSocket(int index);
void CloseSocket(int index);
char* RecvData(int index);

Game *game = nullptr;
using namespace std;

int AcceptSocket(int index) {
    if(client[index]) {
        fprintf(stderr, "ER: Overriding socket at index %d.n", index);
        CloseSocket(index);
    }
 
    client[index] = SDLNet_TCP_Accept(server);
    if(client[index] == NULL) return 0;
 
    if(SDLNet_TCP_AddSocket(socket_set, client[index]) == -1) {
        fprintf(stderr, "ER: SDLNet_TCP_AddSocket: %sn", SDLNet_GetError());
        exit(-1);
    }
 
    return 1;
}

char* RecvData(int index)
{
    char* temp;
    int  num_recv  = SDLNet_TCP_Recv(client[index], temp, 100);
    if(num_recv <= 0)
    { 
        CloseSocket(index);
        const char* err = SDLNet_GetError();
        if(strlen(err) == 0)
        {
            cout << "Client " << index + 1 << " Disconnected" << endl;
        }
        return NULL;
    }
    else
    {
        return temp;
    }
}

void CloseSocket(int index) 
{
    if(client[index] == NULL) 
    {
        fprintf(stderr, "ER: Attempted to delete a NULL socket.n");
        return;
    }
 
    if(SDLNet_TCP_DelSocket(socket_set, client[index]) == -1) 
    {
        fprintf(stderr, "ER: SDLNet_TCP_DelSocket: %sn", SDLNet_GetError());
        exit(-1);
    }
    cout << "Client " << index + 1 << " Closed" << endl;
    SDLNet_TCP_Close(client[index]);
    client[index] = NULL;
}

char sendInt(int first)
{
    char frame = first;
    return frame;
}
char sendBool(bool flag)
{
    if(flag)
    {
        return '1';
    }
    else
    {
        return '0';
    }
}
vector<char> sendRect(SDL_Rect Rect)
{
    vector<char> temp;
    char x = Rect.x;
    char y = Rect.y;
    char w = Rect.w;
    char h = Rect.h;
    temp.push_back(x);
    temp.push_back(y);
    temp.push_back(w);
    temp.push_back(h);
    return temp;    
}

int main(int argc, const char* argv[])
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
    SDLNet_ResolveHost(&ip, NULL, PORT);
    
    server = SDLNet_TCP_Open(&ip);
    socket_set = SDLNet_AllocSocketSet(2);
    if(SDLNet_TCP_AddSocket(socket_set, server) == -1) 
    {
        fprintf(stderr, "ER: SDLNet_TCP_AddSocket: %sn", SDLNet_GetError());
        exit(-1);
    }

    std::cout << "server connected" << std::endl;
    int count = 0;
    
    
    game = new Game();
    game->init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 950, false);
    
    game->loadMedia();

    while(game->running())
    {
        frameStart = SDL_GetTicks();
        
        
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;

        if (time_per_frame > frameTime)
        {
            SDL_Delay(time_per_frame - frameTime);
        }

        if(SDLNet_CheckSockets(socket_set, 0) > 0)
        {
            if(SDLNet_SocketReady(server)) 
            {
                int got_socket = AcceptSocket(next_ind);
                if(!got_socket) 
                {
                    continue;
                }
                else
                {
                    std::cout << "\nClient " << next_ind + 1<< " connected" << std::endl;
                }
        //         // NOTE: get a new index
                int chk_count;
                for(chk_count=0; chk_count<MAX_SOCKETS; ++chk_count) {
                    if(client[(next_ind+chk_count)%MAX_SOCKETS] == NULL) break;
                }
            
                next_ind = (next_ind+chk_count)%MAX_SOCKETS;
        //         //printf("DB: new connection (next_ind = %d)n", next_ind);
            }
            if(client[0] == NULL)
            {
                continue;
            }
            for(int ind=0; (ind<MAX_SOCKETS); ++ind) 
            {
                if(client[ind] == NULL) continue;
                
                int send_size = 400;
                char msg_send[send_size];
                const char* text;
                char msg_recv[11];
                string send_string = "";
                
                send_string += glb->mazefile;
                // cout << send_string << endl;
                
                send_string += std::to_string(glb->player_dir);
                
                send_string += std::to_string(glb->player_move);
                
                string temp;

                if(glb->player_X < 10)
                {
                    temp = "00";
                }
                else if (glb->player_X < 100) 
                {
                    temp = "0";
                }
                else
                {
                    temp = "";
                }
                send_string += temp + std::to_string(abs(glb->player_X));

                if(glb->player_Y < 10)
                {
                    temp = "00";
                }
                else if (glb->player_Y < 100) 
                {
                    temp = "0";
                }
                else
                {
                    temp = "";
                }
                send_string += temp + std::to_string(abs(glb->player_Y));

                send_string += std::to_string(glb->enemy1.enemy_dir);
                send_string += std::to_string(glb->enemy2.enemy_dir);
                send_string += std::to_string(glb->enemy3.enemy_dir);
                send_string += std::to_string(glb->enemy4.enemy_dir);

                if(glb->enemy1.enemy_X < 10)
                {
                    temp = "00";
                }
                else if (glb->enemy1.enemy_X < 100) 
                {
                    temp = "0";
                }
                else
                {
                    temp = "";
                }
                send_string += temp + std::to_string(abs(glb->enemy1.enemy_X));

                if(glb->enemy1.enemy_Y < 10)
                {
                    temp = "00";
                }
                else if (glb->enemy1.enemy_Y < 100) 
                {
                    temp = "0";
                }
                else
                {
                    temp = "";
                }
                send_string += temp + std::to_string(abs(glb->enemy1.enemy_Y));

                if(glb->enemy2.enemy_X < 10)
                {
                    temp = "00";
                }
                else if (glb->enemy2.enemy_X < 100) 
                {
                    temp = "0";
                }
                else
                {
                    temp = "";
                }
                send_string += temp + std::to_string(abs(glb->enemy2.enemy_X));

                if(glb->enemy2.enemy_Y < 10)
                {
                    temp = "00";
                }
                else if (glb->enemy2.enemy_Y < 100) 
                {
                    temp = "0";
                }
                else
                {
                    temp = "";
                }
                send_string += temp + std::to_string(abs(glb->enemy2.enemy_Y));

                if(glb->enemy3.enemy_X < 10)
                {
                    temp = "00";
                }
                else if (glb->enemy3.enemy_X < 100) 
                {
                    temp = "0";
                }
                else
                {
                    temp = "";
                }
                send_string += temp + std::to_string(abs(glb->enemy3.enemy_X));

                if(glb->enemy3.enemy_Y < 10)
                {
                    temp = "00";
                }
                else if (glb->enemy3.enemy_Y < 100) 
                {
                    temp = "0";
                }
                else
                {
                    temp = "";
                }
                send_string += temp + std::to_string(abs(glb->enemy3.enemy_Y));

                if(glb->enemy4.enemy_X < 10)
                {
                    temp = "00";
                }
                else if (glb->enemy4.enemy_X < 100) 
                {
                    temp = "0";
                }
                else
                {
                    temp = "";
                }
                send_string += temp + std::to_string(abs(glb->enemy4.enemy_X));

                if(glb->enemy4.enemy_Y < 10)
                {
                    temp = "00";
                }
                else if (glb->enemy4.enemy_Y < 100) 
                {
                    temp = "0";
                }
                else
                {
                    temp = "";
                }
                send_string += temp + std::to_string(abs(glb->enemy4.enemy_Y));

                send_string += std::to_string(glb->player_lifes);

                send_string += std::to_string(glb->player_power_invisble);

                send_string += std::to_string(glb->player_power_invisble_count);

                text = send_string.c_str();

                SDLNet_TCP_Send(client[ind], text , strlen(text));
                
                if(!SDLNet_SocketReady(client[ind])) continue;
                
                int  num_recv  = SDLNet_TCP_Recv(client[ind], msg_recv, 11);
                // text = RecvData(ind);
                int recv_size[7] = {1, 1, 3, 3, 1, 1, 1};
                if(num_recv <= 0)
                {
                    CloseSocket(ind);
                    cout << "Client " << ind+1 << " Disconnected" << endl;
                    continue;
                }

                int start = 0;
                vector<string> vec;
                for(int j = 0; j<7; j++)
                {
                        string out = "";
                        int i;
                        for(i = start; i< start+recv_size[j]; i++)
                        {
                            out += msg_recv[i];
                        }
                        start = i;
                        // cout << out << endl;
                        vec.push_back(out);
                }
                
                if (vec.size() != 0)
                {
                    glb->player2_dir = stoi(vec[0]);
                    glb->player2_move = stoi(vec[1]);
                    glb->player2_X = stoi(vec[2]);
                    glb->player2_Y = stoi(vec[3]);
                    glb->player2_lifes = stoi(vec[4]);
                    glb->player2_power_invisble = stoi(vec[5]);
                    glb->player2_power_invisble_count = stoi(vec[6]);
                }        
            }      
        }

        game->handleEvents();  
       
    }

    game->clean();

    if(SDLNet_TCP_DelSocket(socket_set, server) == -1) 
    {
        fprintf(stderr, "ER: SDLNet_TCP_DelSocket: %sn", SDLNet_GetError());
        exit(-1);
    }
    

    SDLNet_TCP_Close(server);

    for(int i=0; i<MAX_SOCKETS; ++i) 
    {
        if(client[i] == NULL) continue;
        CloseSocket(i);
    }
    SDLNet_FreeSocketSet(socket_set);
    SDLNet_Quit();
    SDL_Quit();
    return 0;
}