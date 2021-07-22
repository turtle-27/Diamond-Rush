#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <cstring>
#include <unistd.h>
#include <termios.h>



#define PORT 1234

using namespace std;

char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}


int main(int argc, char** argv)
{
    //cout << "input: " << getch() << endl;
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
        // getline(cin, msg_send);
        // text = msg_send.c_str();
        char c = getch();
        text = &c;
        
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