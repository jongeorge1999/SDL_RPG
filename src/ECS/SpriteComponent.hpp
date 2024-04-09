#pragma once
#include "Components.hpp"
#include "SDL2/SDL.h"
#include "../TextureManager.hpp"
#include "Animation.hpp"
#include <map>

class SpriteComponent : public Component {
    private:
        TransformComponent* transform;
        SDL_Texture* texture;
        SDL_Rect srcRect, destRect;

        bool animated = false;
        int frames = 0;
        int speed = 100;

    public:

        int animIndex = 0;

        std::map<const char*, Animation> animations;

        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

        SpriteComponent() = default;
        SpriteComponent(const char* path){
            setTex(path);
        }

        SpriteComponent(const char* path, bool isAnimated){
            animated = isAnimated;

            Animation idle = Animation(0, 4, 300);
            Animation walkLR = Animation(1, 4, 300);
            Animation walkD = Animation(2, 4, 300);
            Animation walkU = Animation(3, 4, 300);

            animations.emplace("Idle", idle);
            animations.emplace("WalkLR", walkLR);
            animations.emplace("WalkD", walkD);
            animations.emplace("WalkU", walkU);

            Play("Idle");
            setTex(path);
        }

        ~SpriteComponent(){
            SDL_DestroyTexture(texture);
        }



        void setTex(const char* path) {
            texture = TextureManager::LoadTexture(path);
        }

        void init() override {
            transform = &entity->getComponent<TransformComponent>();
            
            srcRect.x = srcRect.y = 0;
            srcRect.w = transform->width;
            srcRect.h = transform->height;
        }

        void update() override {

            if(animated) {
                srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
            }

            srcRect.y = animIndex * transform->height;

            destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
            destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
            destRect.w = transform->width*transform->scale;
            destRect.h = transform->height*transform->scale;
        }

        void draw() override {
            TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
        }

        void Play(const char* animName) {
            frames = animations[animName].frames;
            animIndex = animations[animName].index;
            speed = animations[animName].speed;
        }
};