# Sentinel — Tower Defense 2D

Jeu de tower defense 2D en C++ avec SFML.  
Projet réalisé dans le cadre du cours de POO — ESIR, Université Rennes 1.

## Concepts C++ utilisés
- Héritage et polymorphisme (`Entity` → `Enemy`, `Tower`, `Projectile`)
- Méthodes virtuelles pures (`update()`, `draw()`)
- Smart pointers (`shared_ptr`)
- Templates (`Vector2<T>`)
- STL (`vector`, `remove_if`, lambdas)
- Règle des 3 (`Grid`)
- Surcharge d'opérateurs (`Vector2`)

## Installation

### Prérequis
```bash
sudo apt install g++ libsfml-dev make
```

### Compiler et lancer
```bash
git clone https://github.com/babourwel2001-cpu/sentinel.git
cd sentinel
make
./sentinel
```

## Contrôles
| Touche | Action |
|--------|--------|
| `1` | Tour Basic (50 or) |
| `2` | Tour Sniper (100 or) |
| `3` | Tour Rapid (75 or) |
| Clic gauche | Poser une tour |
| Croix / Ctrl+C | Quitter |

## Équipe
- Ibrahima
- Ndiaga
- Mohamed
- Mamoudou
- Mack
