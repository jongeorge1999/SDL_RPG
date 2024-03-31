#pragma once
#include "Components.hpp"
#include "SDL2/SDL.h"
#include "../TextureManager.hpp"

class SpriteComponent : public Component {
    private:
        TransformComponent* transform;
        SDL_Texture* texture;
        SDL_Rect srcRect, destRect;

        bool animated = false;
        int frames = 0;
        int speed = 100;

    public:
        SpriteComponent() = default;
        SpriteComponent(const char* path){
            setTex(path);
        }

        SpriteComponent(const char* path, int numFrames, int mSpeed){
            animated = true;
            frames = numFrames;
            speed = mSpeed;
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
                srcRect.y = srcRect.h * static_cast<int>((SDL_GetTicks() / speed) % frames);
            }

            destRect.x = static_cast<int>(transform->position.x);
            destRect.y = static_cast<int>(transform->position.y);
            destRect.w = transform->width*transform->scale;
            destRect.h = transform->height*transform->scale;
        }

        void draw() override {
            TextureManager::Draw(texture, srcRect, destRect);
        }
};