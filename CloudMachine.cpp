
#include "stdafx.h"

#include <time.h>

#include "game.h"
#include "debug.h"

void GameLoop()
{
	// Seed the RNG
	srand(time(NULL));

	// Create the game objects
	GameObjects game;

	// Start the clock
	sf::Clock clock;
	float frameTime = 1.f / 60.f;
	float elapsed;

	// Do the game loop
	while (game.window_.isOpen())
	{
		bool print = true;

		// Run the frame
		if (print)
		{
			elapsed = clock.restart().asSeconds();
		}

		game.HandleInput();

		if (print)
		{
			elapsed = clock.getElapsedTime().asSeconds();
			std::cout << "HandleInput: " << elapsed << std::endl;
		}

		if (print)
		{
			elapsed = clock.restart().asSeconds();
		}

		game.GameLogic();

		if (print)
		{
			elapsed = clock.getElapsedTime().asSeconds();
			std::cout << "GameLogic: " << elapsed << std::endl;
		}

		if (print)
		{
			elapsed = clock.restart().asSeconds();
		}

		game.Draw();		

		if (print)
		{
			elapsed = clock.getElapsedTime().asSeconds();
			std::cout << "Draw: " << elapsed << std::endl;
			std::cout << "--" << std::endl;
		}
	}
}

int main()
{
	GameLoop();

	return 0;
}
