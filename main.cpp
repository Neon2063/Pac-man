#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <array>
#include "maps.hpp"

const int TILE_SIZE = 24;
int playerRow = 0, playerCol = 0;
std::array<std::string, MAP_HEIGHT> currentMap;

void selectMap() {
    std::cout << "Choose a map (1-4): ";
    int choice;
    std::cin >> choice;

    if (choice < 1 || choice > allMaps.size()) {
        std::cout << "Invalid choice. Using map 1.\n";
        choice = 1;
    }

    currentMap = allMaps[choice - 1];

   //player position findd hunxha eta
    for (int row = 0; row < MAP_HEIGHT; ++row) {
        for (int col = 0; col < MAP_WIDTH; ++col) {
            if (currentMap[row][col] == 'P') {
                playerRow = row;
                playerCol = col;
                currentMap[row][col] = ' ';  
                return;
            }
        }
    }
}
// handle input for player movement
void handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    playerRow--;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  playerRow++;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  playerCol--;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) playerCol++;

    // Clamp within map bounds
    if (playerRow < 0) playerRow = 0;
    if (playerRow >= MAP_HEIGHT) playerRow = MAP_HEIGHT - 1;
    if (playerCol < 0) playerCol = 0;
    if (playerCol >= MAP_WIDTH) playerCol = MAP_WIDTH - 1;
}

void drawMap(sf::RenderWindow& window) {
    sf::RectangleShape wall(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    wall.setFillColor(sf::Color::Blue);

    sf::CircleShape pellet(3);
    pellet.setFillColor(sf::Color::White);

    sf::CircleShape powerPellet(6);
    powerPellet.setFillColor(sf::Color::Red);

    for (int row = 0; row < MAP_HEIGHT; ++row) {
        for (int col = 0; col < MAP_WIDTH; ++col) {
            char tile = currentMap[row][col];
            float x = col * TILE_SIZE;
            float y = row * TILE_SIZE;

            switch (tile) {
            case '#':
                wall.setPosition(x, y);
                window.draw(wall);
                break;
            case '.':
                pellet.setPosition(x + TILE_SIZE / 2 - 3, y + TILE_SIZE / 2 - 3);
                window.draw(pellet);
                break;
            case 'o':
                powerPellet.setPosition(x + TILE_SIZE / 2 - 6, y + TILE_SIZE / 2 - 6);
                window.draw(powerPellet);
                break;
                // other characters like '1','2','3','=','0' can be handled later
            default:
                break;
            }
        }
    }
}

int main() {
    selectMap();

    int windowWidth = MAP_WIDTH * TILE_SIZE;
    int windowHeight = MAP_HEIGHT * TILE_SIZE;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Pac-Man");

    sf::CircleShape pacman(TILE_SIZE / 2 - 2);
    pacman.setFillColor(sf::Color::Yellow);

    sf::Clock clock;
    float moveDelay = 0.12f;
    float moveTimer = 0.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();
        moveTimer += dt;

        if (moveTimer > moveDelay) {
            handleInput();
            moveTimer = 0.0f;
        }

        window.clear(sf::Color::Black);
        drawMap(window);

        pacman.setPosition(playerCol * TILE_SIZE + 2, playerRow * TILE_SIZE + 2);
        window.draw(pacman);

        window.display();
    }

    return 0;
}
