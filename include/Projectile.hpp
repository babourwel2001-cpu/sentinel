#pragma once
#include "Entity.hpp"

class Projectile : public Entity {
private:
    Vec2f   direction;
    float   damage;
    float   radius;
    sf::Color color;

public:
    Projectile(Vec2f pos, Vec2f target, float dmg, float spd, sf::Color color)
        : Entity(pos, spd), damage(dmg), radius(5.f), color(color) {
        Vec2f diff = target - pos;
        direction  = diff.normalized();
    }

    ~Projectile() {}

    void update(float dt) override {
        position += direction * (speed * dt);
    }

    void draw(sf::RenderWindow& window) const override {
        sf::CircleShape shape(radius);
        shape.setFillColor(color);
        shape.setPosition(position.x - radius, position.y - radius);
        window.draw(shape);
    }

    float getDamage() const { return damage; }

    bool outOfBounds(float w, float h) const {
        return position.x < 0 || position.x > w
            || position.y < 0 || position.y > h;
    }
};