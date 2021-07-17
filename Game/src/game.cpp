#include "game.hpp"
#include "TextureManager.hpp"
#include "map.hpp"
#include "components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"

Map* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Manager manager;
auto& player(manager.addEntity());
auto& wall(manager.addEntity());


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
        renderer = SDL_CreateRenderer(window, - 1, 0);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }

        isRunning = true;
    }
    
    map = new Map();
    
    player.addComponent<TransformComponent>();
    player.addComponent<KeyboardController>();
    player.addComponent<SpriteComponent>("data/player.png");
    player.addComponent<ColliderComponent>("player");
    

    wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1);
    wall.addComponent<SpriteComponent>("data/dirt.png");
    wall.addComponent<ColliderComponent>("wall");

}

void Game::handleEvents()
{
    SDL_PollEvent(&event);
    switch (event.type)
    {
        case SDL_QUIT:
            isRunning = false;
            break;

        default:
            break;
    }
}

void Game::update()
{
    manager.refresh();
    manager.update();

    if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
                        wall.getComponent<ColliderComponent>().collider))
    {
        player.getComponent<TransformComponent>().velocity * -1;
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);
    map->DrawMap();
    manager.draw();
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}