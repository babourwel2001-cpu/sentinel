#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include "Grid.hpp"
#include "Enemy.hpp"
#include "Tower.hpp"
#include "Projectile.hpp"

enum class TowerType { Basic, Sniper, Rapid };
enum class GameState { Playing, GameOver, Win };

class Game {
private:
    sf::RenderWindow        window;
    sf::Font                font;
    Grid                    grid;

    std::vector<std::shared_ptr<Enemy>>      enemies;
    std::vector<std::shared_ptr<Tower>>      towers;
    std::vector<std::shared_ptr<Projectile>> projectiles;
    std::vector<Vec2f>                       path;

    int         gold;
    int         lives;
    int         wave;
    int         maxWaves;
    float       waveTimer;
    float       spawnTimer;
    int         enemiesToSpawn;
    int         enemiesSpawned;
    bool        waveInProgress;
    GameState   state;
    TowerType   selectedTower;

    sf::Clock   clock;

public:
    Game();
    ~Game() {}

    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    void initPath();
    void spawnWave();
    void spawnEnemy();
    void towerShoot(float dt);
    void checkCollisions();
    void cleanDead();
    void placeTower(int r, int c);
    void drawHUD();
    void drawGameOver();
    void drawWin();

    int  towerCost(TowerType t) const;
    std::string towerName(TowerType t) const;
};