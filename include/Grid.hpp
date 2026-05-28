#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Vector2.hpp"

enum class CellType { Empty, Path, Tower };

class Grid {
private:
    int         rows;
    int         cols;
    int         cellSize;
    std::vector<std::vector<CellType>> cells;

public:
    Grid(int rows, int cols, int cellSize)
        : rows(rows), cols(cols), cellSize(cellSize) {
        cells.resize(rows, std::vector<CellType>(cols, CellType::Empty));
    }

    // Règle des 3
    Grid(const Grid& other)
        : rows(other.rows), cols(other.cols),
          cellSize(other.cellSize), cells(other.cells) {}

    Grid& operator=(const Grid& other) {
        if (this != &other) {
            rows     = other.rows;
            cols     = other.cols;
            cellSize = other.cellSize;
            cells    = other.cells;
        }
        return *this;
    }

    ~Grid() {}

    void setCell(int r, int c, CellType type) {
        if (r >= 0 && r < rows && c >= 0 && c < cols)
            cells[r][c] = type;
    }

    CellType getCell(int r, int c) const {
        if (r >= 0 && r < rows && c >= 0 && c < cols)
            return cells[r][c];
        return CellType::Empty;
    }

    bool isBuildable(int r, int c) const {
        return getCell(r, c) == CellType::Empty;
    }

    Vec2f cellToWorld(int r, int c) const {
        return { (float)(c * cellSize + cellSize / 2),
                 (float)(r * cellSize + cellSize / 2) };
    }

    Vec2i worldToCell(float x, float y) const {
        return { (int)(y / cellSize), (int)(x / cellSize) };
    }

    int getCellSize() const { return cellSize; }
    int getRows()     const { return rows; }
    int getCols()     const { return cols; }

    void draw(sf::RenderWindow& window) const {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
                cell.setPosition(c * cellSize, r * cellSize);
                switch (cells[r][c]) {
                    case CellType::Empty:
                        cell.setFillColor(sf::Color(40, 44, 52));
                        cell.setOutlineColor(sf::Color(60, 64, 72));
                        cell.setOutlineThickness(1);
                        break;
                    case CellType::Path:
                        cell.setFillColor(sf::Color(120, 90, 40));
                        break;
                    case CellType::Tower:
                        cell.setFillColor(sf::Color(30, 34, 42));
                        break;
                }
                window.draw(cell);
            }
        }
    }
};