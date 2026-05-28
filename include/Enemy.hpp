#pragma once
#include "Entity.hpp"
#include <vector>

class Enemy : public Entity {
protected:
    float       maxHp;
    float       hp;
    int         reward;
    sf::Color   color;
    int         pathIndex;

public:
    Enemy(Vec2f pos, float hp, float speed, int reward, sf::Color color)
        : Entity(pos, speed), maxHp(hp), hp(hp),
          reward(reward), color(color), pathIndex(0) {}

    virtual ~Enemy() {}

    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;

    void takeDamage(float dmg) { hp -= dmg; if (hp <= 0) alive = false; }
    bool reachedEnd() const    { return pathIndex >= (int)path.size(); }
    int  getReward()  const    { return reward; }
    float getHp()     const    { return hp; }
    float getMaxHp()  const    { return maxHp; }

    void setPath(const std::vector<Vec2f>& p) { path = p; pathIndex = 0; }

protected:
    std::vector<Vec2f> path;
};

class FastEnemy : public Enemy {
public:
    FastEnemy(Vec2f pos)
        : Enemy(pos, 60.f, 120.f, 15, sf::Color(100, 200, 255)) {}
};

class TankEnemy : public Enemy {
public:
    TankEnemy(Vec2f pos)
        : Enemy(pos, 300.f, 40.f, 50, sf::Color(180, 60, 60)) {}
};

class BasicEnemy : public Enemy {
public:
    BasicEnemy(Vec2f pos)
        : Enemy(pos, 100.f, 70.f, 25, sf::Color(50, 220, 80)) {}
};