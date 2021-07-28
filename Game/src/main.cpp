#include "GameEngine.hpp"
#include "Global.hpp"


// #define PORT 1234
// #define MAX_SOCKETS 2


// TCPsocket server;
// TCPsocket client[2];
// SDLNet_SocketSet socket_set;
// int next_ind = 0;
// int AcceptSocket(int index);
// void CloseSocket(int index);
// char* RecvData(int index);

Game *game = nullptr;
using namespace std;

// int AcceptSocket(int index) {
//     if(client[index]) {
//         fprintf(stderr, "ER: Overriding socket at index %d.n", index);
//         CloseSocket(index);
//     }
 
//     client[index] = SDLNet_TCP_Accept(server);
//     if(client[index] == NULL) return 0;
 
//     if(SDLNet_TCP_AddSocket(socket_set, client[index]) == -1) {
//         fprintf(stderr, "ER: SDLNet_TCP_AddSocket: %sn", SDLNet_GetError());
//         exit(-1);
//     }
 
//     return 1;
// }

// char* RecvData(int index)
// {
//     char* temp;
//     int  num_recv  = SDLNet_TCP_Recv(client[index], temp, 100);
// //     //cout << "num_recv in RecvData for Client" << index << " " << num_recv << endl;
//     if(num_recv <= 0)
//     { 
//         CloseSocket(index);
//         const char* err = SDLNet_GetError();
//         if(strlen(err) == 0)
//         {
//             cout << "Client " << index + 1 << " Disconnected" << endl;
//         }
// //         // else
// //         // {
// //         //     fprintf(stderr, "ERROR: SDLNet_TCP_Recv: %sn", err);
// //         // }
//         return NULL;
//     }
//     else
//     {
//         return temp;
//     }
// }

// void CloseSocket(int index) 
// {
//     if(client[index] == NULL) 
//     {
//         fprintf(stderr, "ER: Attempted to delete a NULL socket.n");
//         return;
//     }
 
//     if(SDLNet_TCP_DelSocket(socket_set, client[index]) == -1) 
//     {
//         fprintf(stderr, "ER: SDLNet_TCP_DelSocket: %sn", SDLNet_GetError());
//         exit(-1);
//     }
//     cout << "Client " << index + 1 << " Closed" << endl;
//     SDLNet_TCP_Close(client[index]);
//     client[index] = NULL;
// }


int main(int argc, const char* argv[])
{
    const int FPS = 60;
    const int time_per_frame = 1000/FPS; 

    Uint32 frameStart;
    int frameTime;

    SDL_Init(SDL_INIT_EVERYTHING);
    // SDLNet_Init();
    
    // IPaddress ip;
    // SDLNet_ResolveHost(&ip, NULL, PORT);
    
    // server = SDLNet_TCP_Open(&ip);
    // socket_set = SDLNet_AllocSocketSet(3);
    // if(SDLNet_TCP_AddSocket(socket_set, server) == -1) 
    // {
    //     fprintf(stderr, "ER: SDLNet_TCP_AddSocket: %sn", SDLNet_GetError());
    //     exit(-1);
    // }
    


    // // // string msg_send;
    // // // const char* text;
    // // // char msg_recv[100];

    // std::cout << "server connected" << std::endl;
    // int count = 0;
    // // // while(true)
    // // // {
    // // // }
    

    
    game = new Game();
    
    // while(game->quit)
    // {
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

            // // NETWORKING

            // // //client = SDLNet_TCP_Accept(server);
            // // //cout << "line 44" << endl;
            // int num_ready = SDLNet_CheckSockets(socket_set, 1000);
            // // //cout << "num ready " << num_ready << " count" << count << endl;
            // // //count++;
            // if(num_ready > 0)
            // {
            // //     //cout << "num ready " << num_ready << endl ;
            //     if(SDLNet_SocketReady(server)) 
            //     {
            //         int got_socket = AcceptSocket(next_ind);
            //         if(!got_socket) 
            //         {
            //             num_ready--;
            //             continue;
            //         }
            //         else
            //         {
            //             std::cout << "\nClient " << next_ind + 1<< " connected" << std::endl;
            //         }
            // //         // NOTE: get a new index
            //         int chk_count;
            //         for(chk_count=0; chk_count<MAX_SOCKETS; ++chk_count) {
            //             if(client[(next_ind+chk_count)%MAX_SOCKETS] == NULL) break;
            //         }

            //         next_ind = (next_ind+chk_count)%MAX_SOCKETS;
            // //         //printf("DB: new connection (next_ind = %d)n", next_ind);

            //         num_ready--;
            //     }
            // }
            // for(int ind=0; (ind<MAX_SOCKETS) && num_ready; ++ind) 
            // {
            //         if(client[ind] == NULL) continue;
            //         if(!SDLNet_SocketReady(client[ind])) continue;

            //         std::string msg_send;
            //         char* text;
            //         char msg_recv[100];
            //         input_struct1 inp_str;
            //         input_struct2 inp_str2;

            //         text = RecvData(ind);
            //         if(ind == 0)
            //         {
            //             //cout << "183.main" << endl;
            //             inp_str.text_p1 = text;
            //         }
            //         else
            //         {
            //             //cout << "189" << endl;
            //             inp_str2.text_p2 = text;
            //         }
            //         if(text == NULL)
            //         {
            //             num_ready--;
            //             continue;
            //         }
            //         std::cout << "\nClient" << ind + 1 <<": " << *text;
            //         // printf(text);
            //         if(*text == 'q')
            //         {
            //             std::cout << "Session Terminated." << std::endl;
            //             CloseSocket(ind);
            //             break;
            //         }

                    game->handleEvents();

            }

            game->clean();
        // }
    
    
        /*if(client)
        {
            cout << "Client Connected." << endl;
            while(true)
            {
        //         // cout << "Server: ";
        //         // getline(cin, msg_send);
        //         // text = msg_send.c_str();

        //         // SDLNet_TCP_Send(client, text, strlen(text)+1);

        //         // if(strcmp(text, "quit") == 0)
        //         // {
        //         //     cout << "Session Terminated." << endl;
        //         //     SDLNet_TCP_Close(client);
        //         //     break;
        //         // }

                SDLNet_TCP_Recv(client, msg_recv, 100);
                cout << "Client: " << msg_recv << endl;
                if(strcmp(msg_recv , "quit") == 0)
                {
                    cout << "Session Terminated." << endl;
                    SDLNet_TCP_Close(client);
                    break;
                }

                
            }

            break;
        }*/


        
        
    

    

    // if(SDLNet_TCP_DelSocket(socket_set, server) == -1) 
    // {
    //     fprintf(stderr, "ER: SDLNet_TCP_DelSocket: %sn", SDLNet_GetError());
    //     exit(-1);
    // }
    

    // SDLNet_TCP_Close(server);

    // for(int i=0; i<MAX_SOCKETS; ++i) 
    // {
    //     if(client[i] == NULL) continue;
    //     CloseSocket(i);
    // }
    // SDLNet_FreeSocketSet(socket_set);
    // SDLNet_Quit();
    SDL_Quit();

    return 0;
}