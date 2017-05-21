#pragma once

#include <RealVec.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <GameObjects/ObjectTypeE.h>

enum ObjectShapeE {
    OBJECT_SHAPE_RECT,
    OBJECT_SHAPE_CIRC
};

typedef struct {
    ObjectTypeE objectType;
    std::string name;
    std::string textureName;
    ObjectShapeE objectShape;

    RealVec size;
    float radius;
    float mass;
    bool dynamic;
} ObjectDesc;