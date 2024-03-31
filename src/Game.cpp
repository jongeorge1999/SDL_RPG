#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "ECS/Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

std::vector<ColliderComponent*> Game::colliders;

auto& wall(manager.addEntity());
auto& player(manager.addEntity());

enum groupLabels : std::size_t {
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders
};

Game::Game(){}
Game::~Game(){}

void Game::init(const char* title, int x, int y, int width, int height, bool fullscreen){
    int flags = 0;
    if(fullscreen) {flags = SDL_WINDOW_FULLSCREEN_DESKTOP;}

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Initialized SDL" << std::endl;
        window = SDL_CreateWindow(title, x,y,width,height, flags);
        if(window){std::cout << "Window Created Successfully" << std::endl;}
        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            std::cout << "Renderer Created Successfully" << std::endl;}
        isRunning = true;
    } else {
        isRunning = false;
    }
    map = new Map();

    Map::LoadMap("../assets/16x16.map", 16,16);

    player.addComponent<TransformComponent>(2);
    player.addComponent<SpriteComponent>("../assets/anim_newPlayer.png", 4, 500);
    player.addComponent<KeyBoardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);

    wall.addComponent<TransformComponent>(400.0f, 400.0f, 20, 300, 1);
    wall.addComponent<SpriteComponent>("../assets/dirt.png");
    wall.addComponent<ColliderComponent>("wall");
    wall.addGroup(groupMap);
}

void Game::handleEvents(){
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

void Game::update(){
    manager.refresh();
    manager.update();

    for(auto cc : colliders) {
        Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
    }
}

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

void Game::render(){
    SDL_RenderClear(renderer);
    for(auto& t : tiles) {
        t->draw();
    }
    for(auto& p : players) {
        p->draw();
    }
    for(auto& e : enemies) {
        e->draw();
    }
    SDL_RenderPresent(renderer);
}

void Game::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game exited" << std::endl;
}

void Game::AddTile(int id, int x, int y) {
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(x, y, 32, 32, id);
    tile.addGroup(groupMap);
}

