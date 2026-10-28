#pragma once

#include <SFML/Graphics.hpp>
#include "CellProperties.h"
class World;

class Cell {
private:
	size_t x;
	size_t y;
	CellType type;
	bool updated = false;
	bool moving = true;
	bool inLiquid = false;
	float fractionX;
	float fractionY;

	sf::Vector2f velocity = { 0.f, 0.f };
	sf::Vector2f acceleration = { 0.f, 0.f };
public:
	Cell(size_t x, size_t y, CellType type)
		: x(x), y(y), type(type), fractionX(0.0f), fractionY(0.0f) {
	}

	size_t getX() const { return x; }
	size_t getY() const { return y; }
	CellType getType() const { return type; }
	bool isUpdated() const { return updated; }
	bool isMoving() const { return moving; }

	sf::Vector2f getVelocity() const { return velocity; }
	sf::Vector2f getAcceleration() const { return acceleration; }

	void setType(CellType type);
	void setUpdated(bool updated);

	void setVelocity(sf::Vector2f vel);
	void setAcceleration(sf::Vector2f acc);

	void UpdatePhysics(float deltaTime, World& world);
};