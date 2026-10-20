#include "SFML/Graphics.hpp"
#include "World.h"

World::World(size_t width, size_t height, size_t cellSize) : width(width / cellSize), height(height / cellSize), cellSize(cellSize){
    cells.reserve(width * height / (cellSize * cellSize));

    for (size_t y = 0; y < height / cellSize; ++y) {
        for (size_t x = 0; x < width / cellSize; ++x) {
            cells.emplace_back(x, y, CellType::Empty);
        }
    }
}

Cell* World::getCell(size_t x, size_t y)
{
    if (!InBounds(x, y))
        return nullptr;
    return &getCellById(x + y * width);
}

Cell& World::getCellById(size_t id)
{
    return cells.at(id);
}

bool World::IsOccupied(size_t x, size_t y)
{
    Cell* cell = getCell(x, y);
    return (cell != nullptr) ? cell->getType() != CellType::Empty : 1;
}

Cell* World::MoveCell(Cell& cellToMove, size_t x2, size_t y2)
{
    if (!InBounds(x2, y2) || IsOccupied(x2, y2))
        return nullptr;

    Cell* cellAtPos = getCell(x2, y2);

    cellAtPos->setType(cellToMove.getType());
    cellAtPos->setVelocity(cellToMove.getVelocity());
    cellAtPos->setAcceleration(cellToMove.getAcceleration());

    cellAtPos->setUpdated(true);

    cellToMove.setType(CellType::Empty);
    sf::Vector2f zero(0, 0);
    cellToMove.setVelocity(zero);
    cellToMove.setAcceleration(zero);

    return cellAtPos;
}

void World::ApplyInitialVelocity(Cell* cell, sf::Vector2f vel)
{
    cell->setVelocity(cell->getVelocity() + vel);
}

void World::Update(float deltaTime, float G)
{
    for (auto& cell : cells)
    {
        cell.setUpdated(false);
    }

    for (auto& cell : cells)
    {
        if (cell.getType() == CellType::Empty || cell.isUpdated())
            continue;

        cell.ApplyGravity(0.0f, G);
        cell.UpdatePhysics(deltaTime, *this);
    }
}

void World::DrawWorld(sf::RenderWindow& window)
{
    sf::VertexArray vertices(sf::PrimitiveType::Triangles);

    vertices.resize(cells.size() * 6);
    vertices.clear();

    for (auto& cell : cells)
    {
        if (cell.getType() == CellType::Empty)
            continue;

        float px = cell.getX() * cellSize;
        float py = cell.getY() * cellSize;
        float size = static_cast<float>(cellSize);

        sf::Color color = sf::Color::White;
        if (cell.getType() == CellType::Rock) color = sf::Color::Blue;

        sf::Vector2f topLeft(px, py);
        sf::Vector2f topRight(px + size, py);
        sf::Vector2f bottomRight(px + size, py + size);
        sf::Vector2f bottomLeft(px, py + size);

        vertices.append(sf::Vertex{ topLeft, color });
        vertices.append(sf::Vertex{ topRight, color });
        vertices.append(sf::Vertex{ bottomLeft, color });

        vertices.append(sf::Vertex{ topRight, color });
        vertices.append(sf::Vertex{ bottomRight, color });
        vertices.append(sf::Vertex{ bottomLeft, color });
    }

    window.draw(vertices);
}