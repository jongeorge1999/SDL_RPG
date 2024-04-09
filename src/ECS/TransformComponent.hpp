#pragma once
#include "Components.hpp"
#include "../Vector2D.hpp"

class TransformComponent : public Component{
    public:

        Vector2D position;
        Vector2D velocity;

        int height = 32;
        int width = 32;
        int scale = 1;

        int speed = 3;


        TransformComponent() {
            position.zero();
        }

        TransformComponent(int sc) {
            position.x = 800;
            position.y = 640;
            scale = sc;
        }

        TransformComponent(float x, float y) {
            position.x = x;
            position.y = y;
        }

        TransformComponent(float x, float y, int w, int h, int sc) {
            position.x = x;
            position.y = y;
            width = w;
            height = h;
            scale = sc;
        }

        void init() override {
            velocity.zero();
        }
        void update() override {
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
        }
};