#pragma once
#include "Entity.hpp"
#include "Projectile.hpp"
#include <vector>
#include <memory>

class Tower : public Entity {
protected:
    float range;
    float damage;
    float fireRate;
    float fireCooldown;
    sf::Color color;

public:
    Tower(Vec2f pos, float range, float dmg, float fireRate, sf::Color color)
        : Entity(pos, 0.f), range(range), damage(dmg),
          fireRate(fireRate), fireCooldown(0.f), color(color) {}

    virtual ~Tower() {}

    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;

    virtual std::shared_ptr<Projectile> shoot(Vec2f target);

    float getRange()  const { return range; }
    float getDamage() const { return damage; }

protected:
    bool canShoot() const { return fireCooldown <= 0.f; }
};

class BasicTower : public Tower {
public:
    BasicTower(Vec2f pos)
        : Tower(pos, 120.f, 25.f, 1.5f, sf::Color(80, 80, 220)) {}
    int getCost() const { return 50; }
};

class SniperTower : public Tower {
public:
    SniperTower(Vec2f pos)
        : Tower(pos, 250.f, 80.f, 0.6f, sf::Color(220, 180, 0)) {}
    int getCost() const { return 100; }
};

class RapidTower : public Tower {
public:
    RapidTower(Vec2f pos)
        : Tower(pos, 90.f, 12.f, 4.f, sf::Color(220, 80, 200)) {}
    int getCost() const { return 75; }
};