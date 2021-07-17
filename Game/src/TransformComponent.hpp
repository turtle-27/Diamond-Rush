#pragma once
#include "components.hpp"
#include "Vector2D.hpp"

class TransformComponent : public Component
{
    public:

        Vector2D position;
        Vector2D velocity;

        int height = 130;
        int width = 130;
        int scale = 1;

        int speed = 2;

        TransformComponent()
        {
            position.Zero();
        }
        
        TransformComponent(int s)
        {
            position.Zero();
            scale = s;
        }

        TransformComponent(float x, float y)
        {
            position.x = x; 
            position.y = y;
        }

        TransformComponent(float x, float y, int w, int h, int s)
        {
            position.x = x; 
            position.y = y;
            height = h;
            width = w;
            scale = s;
        }

        void init() override
        {
            velocity.Zero();
        }

        void update() override
        {
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
        }
};