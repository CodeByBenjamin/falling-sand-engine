#pragma once

#include <SFML/Graphics.hpp>
class World;

enum class CellType {
	Empty,
	Rock
};

class Cell {
private:
	size_t x;
	size_t y;
	CellType type;
	bool updated = false;

	sf::Vector2f velocity = { 0.f, 0.f };
	sf::Vector2f acceleration = { 0.f, 0.f };
	float mass = 1.f;
public:
	Cell(size_t x, size_t y, CellType type)
		: x(x), y(y), type(type) {
	}

	size_t getX() const { return x; }
	size_t getY() const { return y; }
	CellType getType() const { return type; }
	bool isUpdated() const { return updated; }

	float getMass() const { return mass; }
	sf::Vector2f getVelocity() const { return velocity; }
	sf::Vector2f getAcceleration() const { return acceleration; }

	void setType(CellType type);
	void setUpdated(bool updated);

	void setVelocity(sf::Vector2f vel);
	void setAcceleration(sf::Vector2f acc);

	void ApplyGravity(float ax, float ay);
	void UpdatePhysics(float deltaTime, World& world);
};