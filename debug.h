// ----------------------------------------------
//			DEBUG.H
// ----------------------------------------------
// Testing functions
// ---------------------------------------------- 

#ifndef SRC_DEBUG
#define SRC_DEBUG

// ----------------------------------------------

#include <iostream>
#include <cstdint>
#include <thread>

#include <SFML/Graphics.hpp>

// ----------------------------------------------

void FPSTest()
{
	sf::RenderWindow window(sf::VideoMode(256, 256), "FPS Test");
	//window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	GameObjects game;

	// Start the clock
	sf::Clock clock;
	float frameTime = 1.f / 60.f;
	float elapsed;

	// Test the rendering
	while (window.isOpen())
	{
		elapsed = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.display();

		elapsed = clock.getElapsedTime().asSeconds();
		std::cout << "Frame time: " << elapsed << std::endl;
		std::cout << "--" << std::endl;
	}
}

// ----------------------------------------------

#endif