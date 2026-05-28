#include "Tower.hpp"
#include <cmath>

void Tower::update(float dt) {
    if (fireCooldown > 0.f)
        fireCooldown -= dt;
}

void Tower::draw(sf::RenderWindow& window) const {
    // Base de la tour
    float size = 20.f;
    sf::RectangleShape base(sf::Vector2f(size, size));
    base.setFillColor(color);
    base.setOrigin(size / 2, size / 2);
    base.setPosition(position.x, position.y);
    window.draw(base);

    // Canon (petit rectangle au centre)
    sf::RectangleShape canon(sf::Vector2f(6.f, 14.f));
    canon.setFillColor(sf::Color(200, 200, 200));
    canon.setOrigin(3.f, 7.f);
    canon.setPosition(position.x, position.y);
    window.draw(canon);

    // Cercle de portée (discret)
    sf::CircleShape rangeCircle(range);
    rangeCircle.setFillColor(sf::Color(255, 255, 255, 10));
    rangeCircle.setOutlineColor(sf::Color(255, 255, 255, 40));
    rangeCircle.setOutlineThickness(1.f);
    rangeCircle.setOrigin(range, range);
    rangeCircle.setPosition(position.x, position.y);
    window.draw(rangeCircle);
}

std::shared_ptr<Projectile> Tower::shoot(Vec2f target) {
    if (!canShoot()) return nullptr;
    fireCooldown = 1.f / fireRate;
    return std::make_shared<Projectile>(
        position, target, damage, 300.f, sf::Color(255, 220, 50)
    );
}