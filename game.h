// ----------------------------------------------
//			GAME.H
// ----------------------------------------------
// Wraps up a game loop and init functions
// ---------------------------------------------- 

#ifndef SRC_GAME
#define SRC_GAME

// ----------------------------------------------

#include <stdlib.h>

#include "perlin.h"
#include "cloud.h"

// ----------------------------------------------

struct GameObjects
{
	int width			= 256;
	int height			= 256;

	sf::RenderWindow	window_;
	
	sf::Image			tilesetImage_;
	sf::Texture			bg_;
	sf::Sprite			bgSprite_;

	CloudMachine		clouds_;

	GameObjects();

	void HandleInput();
	void GameLogic();
	void Draw();
};

// ----------------------------------------------

GameObjects::GameObjects() :
window_(sf::VideoMode(width, height), "Clouds"),
clouds_(25)
{
	// Build the render window
	// window_.setVerticalSyncEnabled(true);
	window_.setFramerateLimit(60);

	// Load the background image
	if (!bg_.loadFromFile("resource/bg.png"))
	{
		std::cout << "Couldn't load background texture." << std::endl;
	}

	// Create a sprite from the image
	bgSprite_ = sf::Sprite(bg_);
	bgSprite_.setPosition(0.0f, 0.0f);

}

// ----------------------------------------------

void GameObjects::HandleInput()
{
	sf::Event event;
	while (window_.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window_.close();
		}
	}
}

void GameObjects::GameLogic()
{
	// Move the clouds
	clouds_.Update();
}

void GameObjects::Draw()
{
	sf::Clock clock;
	float frameTime = 1.f / 60.f;
	float elapsed;

	bool print = false;

	window_.clear();

	// Draw the background
	if (print)
	{
		elapsed = clock.restart().asSeconds();
	}
	
	window_.draw(bgSprite_);

	if (print)
	{
		elapsed = clock.getElapsedTime().asSeconds();
		std::cout << "background: " << elapsed << std::endl;
	}
	
	// Draw the clouds
	if (print)
	{
		elapsed = clock.restart().asSeconds();
	}
	
	clouds_.Render(window_);
	
	if (print)
	{
		elapsed = clock.getElapsedTime().asSeconds();
		std::cout << "clouds: " << elapsed << std::endl;
	}

	// Display everything
	if (print)
	{
		elapsed = clock.restart().asSeconds();
	}
	
	window_.display();

	if (print)
	{
		elapsed = clock.getElapsedTime().asSeconds();
		std::cout << "display: " << elapsed << std::endl;
	}
}

// ----------------------------------------------

#endif