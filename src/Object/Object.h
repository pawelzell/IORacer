#pragma once

#include <Box2D.h>
#include <SFML/Graphics.hpp>

class Object {
public:
    virtual void draw(sf::RenderWindow *window) = 0;

    Object(float scale) : scale_(scale) {}

protected:
    float scale_;
    b2Body *body_;
    sf::Sprite sprite_;
};