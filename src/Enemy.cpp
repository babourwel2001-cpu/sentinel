#include "Enemy.hpp"

void Enemy::update(float dt) {
    if (!alive || path.empty()) return;

    if (pathIndex >= (int)path.size()) {
        alive = false;
        return;
    }

    Vec2f target = path[pathIndex];
    Vec2f diff   = target - position;
    float dist   = diff.length();

    if (dist < 5.f) {
        pathIndex++;
    } else {
        Vec2f dir = diff.normalized();
        position += dir * (speed * dt);
    }
}

void Enemy::draw(sf::RenderWindow& window) const {
    if (!alive) return;

    // Corps de l'ennemi
    float radius = 12.f;
    sf::CircleShape shape(radius);
    shape.setFillColor(color);
    shape.setOrigin(radius, radius);
    shape.setPosition(position.x, position.y);
    window.draw(shape);

    // Barre de vie
    float barW  = 28.f;
    float barH  = 4.f;
    float ratio = hp / maxHp;

    sf::RectangleShape bgBar(sf::Vector2f(barW, barH));
    bgBar.setFillColor(sf::Color(80, 0, 0));
    bgBar.setPosition(position.x - barW / 2, position.y - radius - 8);
    window.draw(bgBar);

    sf::RectangleShape hpBar(sf::Vector2f(barW * ratio, barH));
    hpBar.setFillColor(sf::Color(0, 220, 80));
    hpBar.setPosition(position.x - barW / 2, position.y - radius - 8);
    window.draw(hpBar);
}