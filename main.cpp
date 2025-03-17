#include <iostream>
#include <cmath>
#include <numbers>
#include <vector>

#include "SFML/Graphics.hpp"

#include "Sphere.h"

int main() {

	sf::RenderWindow window(sf::VideoMode(900, 900), "Sphere", sf::Style::Close);
	sf::Event defaultEvent;

	Sphere sph1(sf::Vector2f(450, 450), 200, 32);

	while (window.isOpen()) {

		window.clear();

		if (window.pollEvent(defaultEvent)) {
			if (defaultEvent.type == sf::Event::Closed
				|| (defaultEvent.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
					window.close();
			if (defaultEvent.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				sph1.HorizontalMove(-2);
			if (defaultEvent.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				sph1.HorizontalMove(+2);
		}

		sph1.renderPolygons(&window);
		
		window.display();
		
	}

	return 0;

}
