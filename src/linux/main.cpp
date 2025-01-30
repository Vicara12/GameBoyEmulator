#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

int main() {
    // Dimensions of the window and the grid
    const int windowWidth = 400;
    const int windowHeight = 400;
    const int rows = 10;
    const int cols = 10;
    const int squareSize = windowWidth / cols;  // Adjust size of squares based on window size

    // Example data for colors (values in range [0, 255])
    std::vector<std::vector<int>> colorValues(rows, std::vector<int>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            colorValues[i][j] = (i + j) * 20; // Example color gradient based on position
        }
    }

    // Create a window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Matrix of Colored Squares");

    // Main loop to render the window
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();  // Clear the window

        // Draw each square with a color based on the values in the array
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
                int colorValue = colorValues[i][j];
                square.setFillColor(sf::Color(colorValue, 255 - colorValue, 128));  // RGB color based on value
                square.setPosition(j * squareSize, i * squareSize);
                window.draw(square);
            }
        }

        window.display();  // Display the drawn content
    }

    return 0;
}