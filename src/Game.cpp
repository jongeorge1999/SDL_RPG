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

std::vector<ColliderComponent*> Game::colliders;

bool Game::isRunning = false;

auto& wall(manager.addEntity());
auto& player(manager.addEntity());

const char* mapfile = "../assets/terrain_ss.png";

enum groupLabels : std::size_t {
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders
};

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

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

    Map::LoadMap("../assets/map.map", 25,20);

    player.addComponent<TransformComponent>(1);
    player.addComponent<SpriteComponent>("../assets/Player_NewAnims.png", true);
    player.addComponent<KeyBoardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);

    //wall.addComponent<TransformComponent>(400.0f, 400.0f, 20, 300, 1);
    //wall.addComponent<SpriteComponent>("../assets/dirt.png");
    //wall.addComponent<ColliderComponent>("wall");
    //wall.addGroup(groupMap);
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

    Vector2D playerVelocity = player.getComponent<TransformComponent>().velocity;
    int playerSpeed = player.getComponent<TransformComponent>().speed;

    //for(auto cc : colliders) {
        //Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
    //}

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

void Game::AddTile(int srcX, int srcY, int xpos, int ypos) {
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, mapfile);
    tile.addGroup(groupMap);
}

