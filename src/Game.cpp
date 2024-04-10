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

SDL_Rect Game::camera = {0,0, 1600, 1280};

bool Game::isRunning = false;

auto& player(manager.addEntity());

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
    map = new Map("../assets/terrain_ss.png", 3, 32);
    map->LoadMap("../assets/terrain_collision.map", 25,20);

    player.addComponent<TransformComponent>(2);
    player.addComponent<SpriteComponent>("../assets/Player_NewAnims.png", true);
    player.addComponent<KeyBoardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& colliders(manager.getGroup(Game::groupColliders));

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

    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player.getComponent<TransformComponent>().position;

    manager.refresh();
    manager.update();

    for(auto& c : colliders) {
        SDL_Rect cCollider = c->getComponent<ColliderComponent>().collider;
        if(Collision::AABB(cCollider, playerCol)) {
            std::cout << "HIT" << std::endl;
            player.getComponent<TransformComponent>().position = playerPos;
        }
    }

    camera.x = player.getComponent<TransformComponent>().position.x - 800;
    camera.y = player.getComponent<TransformComponent>().position.y - 640;

    if(camera.x < 0) {camera.x = 0;} 
    if(camera.y < 0) {camera.y = 0;}
    if(camera.x > camera.w) {camera.x = camera.w;}
    if(camera.y > camera.h) {camera.y = camera.h;}
}

void Game::render(){
    SDL_RenderClear(renderer);
    for(auto& t : tiles) {
        t->draw();
    }
    for(auto& c : colliders) {
        c->draw();
    }
    for(auto& p : players) {
        p->draw();
    }
    SDL_RenderPresent(renderer);
}

void Game::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game exited" << std::endl;
}