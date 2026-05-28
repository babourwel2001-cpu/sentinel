#pragma once
#include <SFML/Graphics.hpp>
#include "Vector2.hpp"

class Entity {
protected:
    Vec2f position;
    float speed;
    bool  alive;

public:
    Entity(Vec2f pos, float spd)
        : position(pos), speed(spd), alive(true) {}

    virtual ~Entity() {}

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;

    bool isAlive() const { return alive; }
    void kill()          { alive = false; }
    Vec2f getPosition()  const { return position; }

    float distanceTo(const Entity& other) const {
        Vec2f d = position - other.position;
        return d.length();
    }
};