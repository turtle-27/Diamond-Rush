#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <cstring>
 
#define PORT 1234

using namespace std;

int main(int argc,char** argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();
    
    IPaddress ip;
    SDLNet_ResolveHost(&ip, NULL, PORT);
    
    TCPsocket server = SDLNet_TCP_Open(&ip);
    TCPsocket client;
    
    string msg_send;
    const char* text;
    char msg_recv[100];

    
    while(true)
    {
        client = SDLNet_TCP_Accept(server);
    
        if(client)
        {
            cout << "Client Connected." << endl;
            while(true)
            {
                cout << "Server: ";
                getline(cin, msg_send);
                text = msg_send.c_str();

                SDLNet_TCP_Send(client, text, strlen(text)+1);

                if(strcmp(text, "quit") == 0)
                {
                    cout << "Session Terminated." << endl;
                    SDLNet_TCP_Close(client);
                    break;
                }

                SDLNet_TCP_Recv(client, msg_recv, 100);

                cout << "Client: " << msg_recv << endl;
            }

            break;
        }
    }
    
    

    SDLNet_TCP_Close(server);
    
    SDLNet_Quit();
    SDL_Quit();
}