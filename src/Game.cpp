#include "Game.hpp"
#include <algorithm>
#include <sstream>

Game::Game()
    : window(sf::VideoMode(900, 620), "Sentinel - Tower Defense",
             sf::Style::Titlebar | sf::Style::Close)
    , grid(13, 15, 60)
    , gold(200)
    , lives(20)
    , wave(0)
    , maxWaves(5)
    , waveTimer(3.f)
    , spawnTimer(0.f)
    , enemiesToSpawn(0)
    , enemiesSpawned(0)
    , waveInProgress(false)
    , state(GameState::Playing)
    , selectedTower(TowerType::Basic)
{
    window.setFramerateLimit(60);

    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
        if (!font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"))
            font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf");

    initPath();
}

void Game::initPath() {
    // Définition du chemin sur la grille (colonne, ligne)
    std::vector<Vec2i> waypoints = {
        {1, 0}, {1, 4}, {4, 4}, {4, 9},
        {9, 9}, {9, 3}, {12, 3}, {12, 12}, {1, 12}
    };

    for (auto& wp : waypoints)
        grid.setCell(wp.x, wp.y, CellType::Path);

    // Relier les waypoints pour marquer le chemin complet
    for (int i = 0; i + 1 < (int)waypoints.size(); i++) {
        Vec2i a = waypoints[i];
        Vec2i b = waypoints[i + 1];
        int dr = (b.x > a.x) ? 1 : (b.x < a.x) ? -1 : 0;
        int dc = (b.y > a.y) ? 1 : (b.y < a.y) ? -1 : 0;
        int r = a.x, c = a.y;
        while (r != b.x || c != b.y) {
            grid.setCell(r, c, CellType::Path);
            r += dr; c += dc;
        }
    }

    // Construire le chemin en coordonnées monde
    for (auto& wp : waypoints)
        path.push_back(grid.cellToWorld(wp.x, wp.y));
}

void Game::run() {
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        if (dt > 0.05f) dt = 0.05f;

        processEvents();
        if (state == GameState::Playing)
            update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Num1)
                selectedTower = TowerType::Basic;
            if (event.key.code == sf::Keyboard::Num2)
                selectedTower = TowerType::Sniper;
            if (event.key.code == sf::Keyboard::Num3)
                selectedTower = TowerType::Rapid;
            if (event.key.code == sf::Keyboard::R &&
                state != GameState::Playing)
                window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left &&
            state == GameState::Playing) {
            int mx = event.mouseButton.x;
            int my = event.mouseButton.y;
            if (mx < grid.getCols() * grid.getCellSize()) {
                Vec2i cell = grid.worldToCell(mx, my);
                placeTower(cell.x, cell.y);
            }
        }
    }
}
void Game::update(float dt) {
    // Gestion des vagues
    if (!waveInProgress) {
        waveTimer -= dt;
        if (waveTimer <= 0.f && wave < maxWaves) {
            spawnWave();
        }
    }

    // Spawn des ennemis
    if (waveInProgress && enemiesSpawned < enemiesToSpawn) {
        spawnTimer -= dt;
        if (spawnTimer <= 0.f) {
            spawnEnemy();
            enemiesSpawned++;
            spawnTimer = 1.2f;
        }
    }

    // Update ennemis
    for (auto& e : enemies) {
        e->update(dt);
        if (e->isAlive() && e->reachedEnd()) {
            e->kill();
            lives--;
            if (lives <= 0) {
                state = GameState::GameOver;
                return;
            }
        }
    }

    // Update tours + tir
    for (auto& t : towers)
        t->update(dt);
    towerShoot(dt);

    // Update projectiles
    for (auto& p : projectiles)
        p->update(dt);

    checkCollisions();
    cleanDead();

    // Fin de vague
    if (waveInProgress && enemiesSpawned >= enemiesToSpawn && enemies.empty()) {
        waveInProgress = false;
        waveTimer = 5.f;
        gold += 30;
        if (wave >= maxWaves)
            state = GameState::Win;
    }
}

void Game::spawnWave() {
    wave++;
    enemiesSpawned  = 0;
    enemiesToSpawn  = 4 + wave * 2;
    spawnTimer      = 0.f;
    waveInProgress  = true;
}

void Game::spawnEnemy() {
    std::shared_ptr<Enemy> e;
    int type = (wave <= 1) ? 0 : (enemiesSpawned % 3);

    Vec2f startPos = path.front();

    if (type == 0)      e = std::make_shared<BasicEnemy>(startPos);
    else if (type == 1) e = std::make_shared<FastEnemy>(startPos);
    else                e = std::make_shared<TankEnemy>(startPos);

    e->setPath(path);
    enemies.push_back(e);
}

void Game::towerShoot(float dt) {
    for (auto& tower : towers) {
        // Trouver l'ennemi le plus avancé dans la portée
        std::shared_ptr<Enemy> target = nullptr;
        int bestIndex = -1;

        for (auto& e : enemies) {
            if (!e->isAlive()) continue;
            Vec2f diff = e->getPosition() - tower->getPosition();
            if (diff.length() <= tower->getRange()) {
                // On cible l'ennemi le plus avancé sur le chemin
                int idx = 0;
                for (int i = 0; i < (int)enemies.size(); i++) {
                    if (enemies[i] == e) { idx = i; break; }
                }
                if (idx > bestIndex) {
                    bestIndex = idx;
                    target = e;
                }
            }
        }

        if (target) {
            auto proj = tower->shoot(target->getPosition());
            if (proj) projectiles.push_back(proj);
        }
    }
}

void Game::checkCollisions() {
    for (auto& proj : projectiles) {
        if (!proj->isAlive()) continue;
        if (proj->outOfBounds(900.f, 620.f)) {
            proj->kill();
            continue;
        }
        for (auto& e : enemies) {
            if (!e->isAlive()) continue;
            Vec2f diff = proj->getPosition() - e->getPosition();
            if (diff.length() < 14.f) {
                e->takeDamage(proj->getDamage());
                if (!e->isAlive())
                    gold += e->getReward();
                proj->kill();
                break;
            }
        }
    }
}

void Game::cleanDead() {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::shared_ptr<Enemy>& e) { return !e->isAlive(); }),
        enemies.end());

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const std::shared_ptr<Projectile>& p) { return !p->isAlive(); }),
        projectiles.end());
}

void Game::placeTower(int r, int c) {
    if (!grid.isBuildable(r, c)) return;
    int cost = towerCost(selectedTower);
    if (gold < cost) return;

    Vec2f pos = grid.cellToWorld(r, c);
    std::shared_ptr<Tower> t;

    if (selectedTower == TowerType::Basic)
        t = std::make_shared<BasicTower>(pos);
    else if (selectedTower == TowerType::Sniper)
        t = std::make_shared<SniperTower>(pos);
    else
        t = std::make_shared<RapidTower>(pos);

    towers.push_back(t);
    grid.setCell(r, c, CellType::Tower);
    gold -= cost;
}

int Game::towerCost(TowerType t) const {
    if (t == TowerType::Basic)  return 50;
    if (t == TowerType::Sniper) return 100;
    return 75;
}

std::string Game::towerName(TowerType t) const {
    if (t == TowerType::Basic)  return "Basic (50g)";
    if (t == TowerType::Sniper) return "Sniper (100g)";
    return "Rapid (75g)";
}

void Game::render() {
    window.clear(sf::Color(20, 22, 30));
    grid.draw(window);

    for (auto& t    : towers)      t->draw(window);
    for (auto& e    : enemies)     e->draw(window);
    for (auto& p    : projectiles) p->draw(window);

    drawHUD();

    if (state == GameState::GameOver) drawGameOver();
    if (state == GameState::Win)      drawWin();

    window.display();
}

void Game::drawHUD() {
    // Panneau HUD à droite
    sf::RectangleShape panel(sf::Vector2f(900 - grid.getCols() * grid.getCellSize(), 620));
    // Pas de panneau séparé, on dessine sur la zone disponible

    auto makeText = [&](const std::string& str, float x, float y,
                        unsigned size = 14, sf::Color col = sf::Color::White) {
        sf::Text t;
        t.setFont(font);
        t.setString(str);
        t.setCharacterSize(size);
        t.setFillColor(col);
        t.setPosition(x, y);
        window.draw(t);
    };

    float hx = 10.f;
    float hy = 530.f;

    makeText("Or: " + std::to_string(gold),  hx,        hy,      16, sf::Color(255, 215, 0));
    makeText("Vies: " + std::to_string(lives), hx + 120, hy,      16, sf::Color(255, 80, 80));
    makeText("Vague: " + std::to_string(wave) + "/" + std::to_string(maxWaves),
             hx + 250, hy, 16, sf::Color(100, 200, 255));

    if (!waveInProgress && wave < maxWaves) {
        std::ostringstream oss;
        oss << "Prochaine vague dans " << (int)waveTimer + 1 << "s";
        makeText(oss.str(), hx + 400, hy, 14, sf::Color(180, 180, 180));
    }

    // Sélection de tour
    makeText("[1] Basic  [2] Sniper  [3] Rapid", hx, hy + 24, 13, sf::Color(180, 180, 180));
    makeText("Selectionne: " + towerName(selectedTower), hx, hy + 44, 13, sf::Color(100, 255, 150));
}

void Game::drawGameOver() {
    sf::RectangleShape overlay(sf::Vector2f(900, 620));
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(overlay);

    sf::Text t;
    t.setFont(font);
    t.setCharacterSize(48);
    t.setFillColor(sf::Color(255, 60, 60));
    t.setString("GAME OVER");
    t.setPosition(280.f, 240.f);
    window.draw(t);

    sf::Text sub;
    sub.setFont(font);
    sub.setCharacterSize(20);
    sub.setFillColor(sf::Color::White);
    sub.setString("Appuyez sur R pour quitter");
    sub.setPosition(300.f, 310.f);
    window.draw(sub);
}

void Game::drawWin() {
    sf::RectangleShape overlay(sf::Vector2f(900, 620));
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(overlay);

    sf::Text t;
    t.setFont(font);
    t.setCharacterSize(48);
    t.setFillColor(sf::Color(100, 255, 100));
    t.setString("VICTOIRE !");
    t.setPosition(300.f, 240.f);
    window.draw(t);

    sf::Text sub;
    sub.setFont(font);
    sub.setCharacterSize(20);
    sub.setFillColor(sf::Color::White);
    sub.setString("Toutes les vagues repoussees !");
    sub.setPosition(270.f, 310.f);
    window.draw(sub);
}