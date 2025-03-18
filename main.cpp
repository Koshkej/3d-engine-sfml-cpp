#include <iostream>
#include <cmath>
#include <numbers>
#include <vector>
#include "SFML/Graphics.hpp"
#include "Sphere.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(900, 900), "Sphere", sf::Style::Close);
    sf::Event event;
    Sphere sph1(sf::Vector2f(450, 450), 200, 32);

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Right) sph1.HorizontalMove(-2);
                if (event.key.code == sf::Keyboard::Left) sph1.HorizontalMove(+2);
            }
        }

        window.clear();
        sph1.renderPolygons(&window);
        window.display();
    }

    return 0;
}
