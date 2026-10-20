#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "World.h"
#include "Cell.h"

int main()
{
    std::srand(std::time(nullptr));

    float fpsTimer = 0.0f;
    int frameCounter = 0;

    static const unsigned int width = 768u;
    static const unsigned int height = 432u;
    const size_t cellSize = 6;

    sf::Clock clock;
    static const float G = 30;

    bool setup = true;
    World world(width, height, cellSize);

    auto window = sf::RenderWindow(sf::VideoMode({ width, height }), "Falling sand simulation");
    window.setFramerateLimit(60);

    enum simMode
    {
        Drawing,
        Velocity
    };

    simMode mode = Drawing;

    sf::Vector2i mousePos;
    unsigned short mouseRadius = 3;

    sf::Vector2i clickStartPos;

    sf::RectangleShape mousePreview(sf::Vector2f(cellSize, cellSize));
    mousePreview.setFillColor(sf::Color(255, 100, 100, 100));
    mousePreview.setOrigin(sf::Vector2f(cellSize / 2.0f, cellSize / 2.0f));

    bool isDrawing = false;
    bool addingVelocity = false;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        fpsTimer += dt;
        frameCounter++;

        if (fpsTimer >= 1.0f)
        {
            window.setTitle("Falling sand simulation - FPS: " + std::to_string(frameCounter));
            frameCounter = 0;
            fpsTimer -= 1.0f;
        }

        mousePos = sf::Mouse::getPosition(window);

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto* scroll = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                if (scroll->delta > 0)
                {
                    mouseRadius = fmin(mouseRadius + 1, 7);
                }
                else if (scroll->delta < 0)
                {
                    mouseRadius = fmax(mouseRadius - 1, 0);
                }
            }

            if (const auto* mouse_press = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouse_press->button == sf::Mouse::Button::Left)
                {
                    clickStartPos = { mouse_press->position.x, mouse_press->position.y };

                    if (mode == Drawing)
                    {
                        isDrawing = true;
                    }
                    else if (mode == Velocity)
                    {
                        addingVelocity = true;
                    }
                }
            }

            if (const auto* mouse_release = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouse_release->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2i clickEndPos = { mouse_release->position.x, mouse_release->position.y };

                    if (mode == Drawing)
                    {
                        isDrawing = false;
                    }
                    else if (mode == Velocity)
                    {
                        addingVelocity = false;

                        sf::Vector2f throwVector(
                            static_cast<float>(clickStartPos.x - clickEndPos.x),
                            static_cast<float>(clickStartPos.y - clickEndPos.y)
                        );

                        sf::Vector2f initialVelocity = throwVector / 8.0f;

                        int gridMouseX = clickStartPos.x / cellSize;
                        int gridMouseY = clickStartPos.y / cellSize;

                        for (int dy = -mouseRadius; dy <= mouseRadius; dy++)
                        {
                            for (int dx = -mouseRadius; dx <= mouseRadius; dx++)
                            {
                                int cellX = gridMouseX + dx;
                                int cellY = gridMouseY + dy;

                                if (world.InBounds(cellX, cellY))
                                {
                                    Cell* cell = world.getCell(cellX, cellY);
                                    if (cell->getType() != CellType::Empty) {
                                        world.ApplyInitialVelocity(cell, initialVelocity);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                switch (key->scancode)
                {
                    case sf::Keyboard::Scancode::Num1:
                    isDrawing = false;
                    mode = Drawing;
                    break;

                case sf::Keyboard::Scancode::Num2:
                    isDrawing = false;
                    mode = Velocity;
                    break;

                default:
                    break;
                }
            }
        }

        if (isDrawing)
        {
            int gridMouseX = mousePos.x / cellSize;
            int gridMouseY = mousePos.y / cellSize;

            for (int dy = -mouseRadius; dy <= mouseRadius; dy++)
            {
                for (int dx = -mouseRadius; dx <= mouseRadius; dx++)
                {
                    int cellX = gridMouseX + dx;
                    int cellY = gridMouseY + dy;

                    if (world.InBounds(cellX, cellY))
                    {
                        Cell* cell = world.getCell(cellX, cellY);
                        if (cell->getType() == CellType::Empty) {
                            cell->setType(CellType::Rock);
                        }
                    }
                }
            }
        }
        else
        {
            if (!addingVelocity)
            {
                world.Update(dt, G);
            }
        }

        float sideLength = (mouseRadius * 2 + 1) * cellSize;

        mousePreview.setSize(sf::Vector2f(sideLength, sideLength));

        mousePreview.setOrigin(sf::Vector2f(sideLength / 2.0f, sideLength / 2.0f));
        mousePreview.setPosition(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)));

        window.clear(sf::Color::Black);

        world.DrawWorld(window);
        window.draw(mousePreview);
        window.display();
    }
}
