#pragma once

#include "Cell.h"

class World {
private:
	size_t width;
	size_t height;
	size_t cellSize;
	std::vector<Cell> cells;
public:
	World(size_t width, size_t height, size_t cellSize);
	
	Cell* getCell(size_t x, size_t y);
	Cell& getCellById(size_t Id);

	bool InBounds(size_t x, size_t y) const { return x >= 0 && y >= 0 && x < width && y < height; };
	bool IsOccupied(size_t x, size_t y);
	Cell* MoveCell(Cell& cellToMove, size_t x2, size_t y2);

	void ApplyInitialVelocity(Cell* cell, sf::Vector2f vel);
	void Update(float deltaTime, float G);

	void DrawWorld(sf::RenderWindow& window);
};