#include "Cell.h"
#include "World.h"
#include "Constants.h"

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

void Cell::UpdatePhysics(float deltaTime, World& world)
{
    velocity.x += (this->acceleration.x) * deltaTime;
    velocity.y += (this->acceleration.y + cfg::GRAVITY) * deltaTime;

    const CellProperties properties = world.getCellProperties(type);

    //Friction
    velocity *= (1.0f - properties.friction * deltaTime);

    //Liquid friction
    velocity *= (1.0f - (inLiquid ? properties.friction * 0.5f * deltaTime : 0.0f));

    fractionX += velocity.x * deltaTime;
    fractionY += velocity.y * deltaTime;

    float roundedX = 0;
    float roundedY = 0;
    
    if (fractionX >= 1 || fractionX <= -1) {
        roundedX = static_cast<float>(std::floor(fractionX));
        fractionX -= roundedX;
    }

    if (fractionY >= 1 || fractionY <= -1) {
        roundedY = static_cast<float>(std::floor(fractionY));
        fractionY -= roundedY;
    }

    const int maxX = static_cast<int>(world.getWidth()) - 1;
    const int maxY = static_cast<int>(world.getHeight()) - 1;

    const int targetX = std::clamp(static_cast<int>(x) + static_cast<int>(roundedX), 0, maxX);
    const int targetY = std::clamp(static_cast<int>(y) + static_cast<int>(roundedY), 0, maxY);

    if (targetX == x && targetY == y) {
        acceleration = { 0,0 };
        return;
    }

    int x0 = x;
    int y0 = y;
    int x1 = targetX;
    int y1 = targetY;
    size_t pastCellX = x0;
    size_t pastCellY = y0;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    while (true) {
        Cell* c = world.getCell(x0, y0);    

        if (c == nullptr) {
            moving = false;

            Cell *cell = world.SwapCells(*this, pastCellX, pastCellY);
            cell->setUpdated(true);
            break;
        }

        if (c != this && c->getType() != CellType::Air)
        {
            if (properties.density > world.getCellProperties(c->getType()).density) {
                world.SwapCells(*this, *c);
                break;
            } else {
                if (moving) {
                    velocity.x =
                        properties.velocityTransfer * velocity.y * (world.getCoin() ? 1 : -1);

                    if (!c->isMoving()) {
                        moving = false;
                        velocity.x = properties.bounce * velocity.y * (world.getCoin() ? 1 : -1);
                    }
                }

                Cell *cell = world.SwapCells(*this, pastCellX, pastCellY);
                cell->setUpdated(true);
                break;
            }
        }

        if (x0 == x1 && y0 == y1)
        {
            moving = true;
            world.SwapCells(*this, x1, y1);
            setUpdated(true);
            break;
        }

        int e2 = 2 * err;

        pastCellX = x0; pastCellY = y0;

        if (e2 > -dy) { 
            err -= dy; 
            x0 += sx; 
        }
        if (e2 < dx) { 
            err += dx; 
            y0 += sy; 
        }
    }
}