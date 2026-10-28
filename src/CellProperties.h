#pragma once
#include <SFML/Graphics/Color.hpp>
#include <cstdint>

enum class CellType : uint8_t {
    Air,
    Sand,
    Water,
    Count
};

struct CellProperties {
    sf::Color color{ 0, 0, 0, 0 };
    float velocityTransfer = 0.0f;
    float bounce = 0.0f;
    float friction = 0.0f;
	float density = 0.0f;
};