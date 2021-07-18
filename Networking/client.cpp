#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <cstring>

#define PORT 1234

using namespace std;

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();

    IPaddress ip;
    SDLNet_ResolveHost(&ip, "127.0.0.1" , PORT);
    
    TCPsocket client = SDLNet_TCP_Open(&ip);

    string msg_send;
    const char* text;
    char msg_recv[100];

    while(true)
    {
        // SDLNet_TCP_Recv(client, msg_recv, 100);
        
        // cout << "Server: " << msg_recv << endl;

        // if(strcmp(msg_recv, "quit") == 0)
        // {
        //     cout << "Session Terminated." << endl;
        //     break;
        // }

        cout << "Client: ";
        getline(cin, msg_send);
        text = msg_send.c_str();
        
        SDLNet_TCP_Send(client, text, strlen(text)+1);
        if(strcmp(text, "quit") == 0)
        {
            cout << "Session Terminated." << endl;
            break;
        }
    }
    
    SDLNet_TCP_Close(client);
    
    SDLNet_Quit();
    SDL_Quit;
}