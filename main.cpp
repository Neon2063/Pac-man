#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

const int TILE_SIZE = 24;
const int MAP_WIDTH = 28;
const int MAP_HEIGHT = 31;
const int WINDOW_WIDTH = TILE_SIZE * MAP_WIDTH;
const int WINDOW_HEIGHT = TILE_SIZE * MAP_HEIGHT;

const int BUTTON_WIDTH = 200;
const int BUTTON_HEIGHT = 60;
const int BUTTON_GAP = 20;

// Load the selected map (your existing function)
std::vector<std::string> loadMap(int mapNumber) {
    std::vector<std::string> map;
    std::ifstream file("maps/map" + std::to_string(mapNumber) + ".txt");
    std::string line;
    while (std::getline(file, line)) {
        map.push_back(line);
    }
    return map;
}

// Draw the map (your existing function)
void drawMap(sf::RenderWindow& window, const std::vector<std::string>& map) {
    for (int row = 0; row < map.size(); ++row) {
        for (int col = 0; col < map[row].size(); ++col) {
            char tile = map[row][col];
            sf::RectangleShape shape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            shape.setPosition(col * TILE_SIZE, row * TILE_SIZE);

            if (tile == '#') {
                shape.setFillColor(sf::Color::Blue);
                window.draw(shape);
            }
            else if (tile == '.') {
                shape.setSize(sf::Vector2f(6, 6));
                shape.setFillColor(sf::Color::White);
                shape.setPosition(col * TILE_SIZE + 9, row * TILE_SIZE + 9);
                window.draw(shape);
            }
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Home Screen");

    // Load background texture
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("C:/Users/user/Downloads/lolpng.jpg")) {
        std::cerr << "Failed to load background!" << std::endl;
    }
    sf::Sprite background(backgroundTexture);
    background.setScale(
        (float)WINDOW_WIDTH / backgroundTexture.getSize().x,
        (float)WINDOW_HEIGHT / backgroundTexture.getSize().y
    );

    // Load your font from absolute path
    sf::Font font;
    if (!font.loadFromFile("C:/Users/user/Documents/static/OpenSans-Regular.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return -1;  // Exit if font not loaded
    }

    // Create buttons
    std::vector<sf::RectangleShape> buttons;
    int totalHeight = 5 * BUTTON_HEIGHT + 4 * BUTTON_GAP;
    int startY = (WINDOW_HEIGHT - totalHeight) / 2;

    for (int i = 0; i < 5; ++i) {
        sf::RectangleShape button(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
        button.setPosition((WINDOW_WIDTH - BUTTON_WIDTH) / 2, startY + i * (BUTTON_HEIGHT + BUTTON_GAP));
        button.setFillColor(i == 4 ? sf::Color(200, 50, 50) : sf::Color(100, 100, 255)); // Exit red
        buttons.push_back(button);
    }

    // Create texts for buttons
    std::vector<sf::Text> buttonTexts;
    std::vector<std::string> labels = { "Map 1", "Map 2", "Map 3", "Map 4", "Exit" };

    for (int i = 0; i < 5; ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(labels[i]);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);

        // Center the text on the button
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);

        sf::Vector2f buttonPos = buttons[i].getPosition();
        text.setPosition(buttonPos.x + BUTTON_WIDTH / 2.0f,
            buttonPos.y + BUTTON_HEIGHT / 2.0f);

        buttonTexts.push_back(text);
    }

    int selectedMap = -1;

    // Menu loop
    while (window.isOpen() && selectedMap == -1) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

                for (int i = 0; i < buttons.size(); ++i) {
                    if (buttons[i].getGlobalBounds().contains(mousePos)) {
                        if (i == 4) {
                            window.close();  // Exit
                        }
                        else {
                            selectedMap = i + 1;
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(background);
        for (auto& button : buttons) window.draw(button);
        for (auto& text : buttonTexts) window.draw(text);
        window.display();
    }

    // If map selected, load and show
    if (selectedMap != -1) {
        std::vector<std::string> currentMap = loadMap(selectedMap);
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear(sf::Color::Black);
            drawMap(window, currentMap);
            window.display();
        }
    }

    return 0;
}