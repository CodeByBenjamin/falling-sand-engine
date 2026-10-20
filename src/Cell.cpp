#include "Cell.h"
#include "World.h"

void Cell::setType(CellType type)
{
    this->type = type;
}

void Cell::setUpdated(bool updated)
{
    this->updated = updated;
}

void Cell::setVelocity(sf::Vector2f vel)
{
    this->velocity.x = vel.x;
    this->velocity.y = vel.y;
}

void Cell::setAcceleration(sf::Vector2f acc)
{
    this->acceleration.x = acc.x;
    this->acceleration.y = acc.y;
}

void Cell::ApplyGravity(float ax, float ay)
{
    acceleration.x += ax;
    acceleration.y += ay;
}

void Cell::UpdatePhysics(float deltaTime, World& world)
{
    velocity.x += acceleration.x * deltaTime;
    velocity.y += acceleration.y * deltaTime;

    int targetX = static_cast<int>(std::round(this->x + velocity.x * deltaTime));
    int targetY = static_cast<int>(std::round(this->y + velocity.y * deltaTime));

    if (targetX == x && targetY == y) {
        acceleration = { 0,0 };
        return;
    }

    int x0 = x;
    int y0 = y;
    int x1 = targetX;
    int y1 = targetY;
    int lastX = x0;
    int lastY = y0;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    while (true) {
        Cell* c = world.getCell(x0, y0);
        if (world.IsOccupied(x0, y0) && c != this)
        {
            Cell* cell = world.MoveCell(*this, lastX, lastY);

            float jiggleStrength = 0.05f;
            float randX = jiggleStrength * (static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f);
            float randY = jiggleStrength * (static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f);
            cell->setVelocity(sf::Vector2f(randX, randY));
            break;
        }

        if (x0 == x1 && y0 == y1)
        {
            world.MoveCell(*this, x1, y1);
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; lastX = x0; x0 += sx; }
        if (e2 < dx) { err += dx; lastY = y0; y0 += sy; }
    }

    acceleration.x = 0;
    acceleration.y = 0;
}