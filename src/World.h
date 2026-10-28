#pragma once

#include "Cell.h"
#include "Rng.h"
#include "CellProperties.h"
#include "MaterialRegistry.h"

class World {
private:
	size_t width;
	size_t height;
	std::vector<Cell> cells;
	
	bool switchUpdate = false;
	Rng rng;
	MaterialRegistry& materialRegistry;
public:
	World(size_t width, size_t height, uint32_t seed, MaterialRegistry& mr);
	
	size_t getWidth() const { return width; };
	size_t getHeight() const { return height; };
	Cell* getCell(size_t x, size_t y);
	Cell& getCellById(size_t Id);

	bool InBounds(size_t x, size_t y) const { return x >= 0 && y >= 0 && x < width && y < height; };
        Cell *SwapCells(Cell &cellToMove, Cell &cellAtPos);
	Cell* SwapCells(Cell& cellToMove, size_t x2, size_t y2);

	void ApplyInitialVelocity(Cell* cell, sf::Vector2f vel);
	void Update(float deltaTime);

	void DrawWorld(sf::RenderWindow& window);

	bool getCoin() { return rng.coin(); };
	float getRandomFloat() { return rng.randFloat(); };

	const CellProperties getCellProperties(CellType type) { return materialRegistry.get(type); };
};