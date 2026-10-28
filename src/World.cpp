#include "SFML/Graphics.hpp"
#include "World.h"
#include "Constants.h"

World::World(size_t width, size_t height, uint32_t seed, MaterialRegistry& mr) : width(width / cfg::CELL_SIZE), height(height / cfg::CELL_SIZE), rng(Rng(seed)), materialRegistry(mr) 
{
    cells.reserve(width * height / (cfg::CELL_SIZE * cfg::CELL_SIZE));

    for (size_t y = 0; y < height / cfg::CELL_SIZE; ++y) {
        for (size_t x = 0; x < width / cfg::CELL_SIZE; ++x) {
            cells.emplace_back(x, y, CellType::Air);
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

Cell* World::SwapCells(Cell& cellToMove, Cell& cellAtPos)
{
    if (!InBounds(cellAtPos.getX(), cellAtPos.getY()))
        return nullptr;

	const CellType tempType = cellAtPos.getType();
	const sf::Vector2f tempVelocity = cellAtPos.getVelocity();
	const sf::Vector2f tempAcceleration = cellAtPos.getAcceleration();

    cellAtPos.setType(cellToMove.getType());
    cellAtPos.setVelocity(cellToMove.getVelocity());
    cellAtPos.setAcceleration(cellToMove.getAcceleration());

    cellToMove.setType(tempType);
    cellToMove.setVelocity(tempVelocity);
    cellToMove.setAcceleration(tempAcceleration);

    return &cellAtPos;
}

Cell *World::SwapCells(Cell &cellToMove, size_t x2, size_t y2) {
    if (!InBounds(x2, y2))
        return nullptr;

    Cell& cellAtPos = getCellById(x2 + y2 * width);

    return SwapCells(cellToMove, cellAtPos);
}

void World::ApplyInitialVelocity(Cell* cell, sf::Vector2f vel)
{
    cell->setVelocity(cell->getVelocity() + vel);
}

void World::Update(float deltaTime)
{
    for (auto& cell : cells)
    {
        cell.setUpdated(false);
    }

    if (!switchUpdate) {
        for (int y = height - 1; y >= 0; --y) {
            for (size_t x = 0; x < width; ++x) {
                Cell* cell = getCell(x, y);

                if (cell == nullptr)
                    continue;

                if (cell->getType() == CellType::Air || cell->isUpdated())
                    continue;

                cell->UpdatePhysics(deltaTime, *this);
            }
        }

        switchUpdate = true;
    }
    else {
        for (int y = height - 1; y >= 0; --y) {
            for (int x = width - 1; x >= 0; --x) {
                Cell* cell = getCell(x, y);

                if (cell == nullptr)
                    continue;

                if (cell->getType() == CellType::Air || cell->isUpdated())
                    continue;

                cell->UpdatePhysics(deltaTime, *this);
            }
        }

        switchUpdate = false;
    }
}

void World::DrawWorld(sf::RenderWindow& window)
{
    sf::VertexArray vertices(sf::PrimitiveType::Triangles);

    vertices.resize(cells.size() * 6);
    vertices.clear();

    for (auto& cell : cells)
    {
        if (cell.getType() == CellType::Air)
            continue;

        float px = cell.getX() * cfg::CELL_SIZE;
        float py = cell.getY() * cfg::CELL_SIZE;
        float size = static_cast<float>(cfg::CELL_SIZE);

		sf::Color color = materialRegistry.get(cell.getType()).color;

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