// ----------------------------------------------
//			CLOUD.H
// ----------------------------------------------
// Procedurally generates and moves cloud textures
// ---------------------------------------------- 

#ifndef SRC_CLOUD
#define SRC_CLOUD

// ----------------------------------------------

#include <iostream>
#include <cstdint>
#include <thread>

#include <SFML/Graphics.hpp>

// ----------------------------------------------

const int CLOUD_WIDTH = 256;
const int CLOUD_HEIGHT = 256;

// ----------------------------------------------

sf::Image GetNoise(int width, int height)
{
	double* noise = PerlinNoise(width, height, 8, 1.0, 0.5);

	sf::Image img;
	sf::Color qwerty(128, 0, 255, 255);
	img.create(width, height, qwerty);

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			double shade = (int)(255 * noise[x + width * y]);
			sf::Color color(shade, shade, shade, 255);
			img.setPixel(x, y, color);
		}
	}

	return img;
}

sf::Image GetNoiseCloud(int width, int height)
{
	double* noise = NoiseCloud(width, height, 8, 1.0, 0.5);

	sf::Image img;
	sf::Color qwerty(128, 0, 255, 255);
	img.create(width, height, qwerty);

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			double shade = (int)(255 * noise[x + width * y]);
			sf::Color color(255, 255, 255, shade);
			img.setPixel(x, y, color);
		}
	}

	return img;
}

// ----------------------------------------------

struct Cloud
{
	sf::Image		img_;
	sf::Texture		tex_;
	sf::Sprite		sprite_;

	void Init();

	Cloud();

	int x();
	int y();
};

// ----------------------------------------------

void Cloud::Init()
{
	img_ = GetNoiseCloud(CLOUD_WIDTH, CLOUD_HEIGHT);
	if (!tex_.loadFromImage(img_))
	{
		std::cout << "WTF" << std::endl;
	}
	sprite_ = sf::Sprite(tex_);
	sprite_.setPosition((rand() % CLOUD_WIDTH) - (CLOUD_WIDTH / 2), -256);
}

Cloud::Cloud()
{
	Init();
}

int Cloud::x()
{
	return sprite_.getPosition().x;
}

int Cloud::y()
{
	return sprite_.getPosition().y;
}

// ----------------------------------------------

class CloudMachine
{
private:
	int length;

	Cloud*		clouds_;			// List of (2 * length) clouds to be processed in the background
	int*		cloud_indices_;		// Indices of on-screen clouds

	std::thread background_;

	bool Onscreen(int i);

	void FixClouds();
	void AddPreppedCloud(int i);
	void BackgroundThread();

public:
	CloudMachine(int n);

	void Update();
	void Render(sf::RenderWindow& window);
};

bool CloudMachine::Onscreen(int i)
{
	// Determines if cloud at raw index i is onscreen and supposed to be

	return (clouds_[i].y() <= 256);

	/*
	for (int j = 0; j < length; j++)
	{
	if (i == cloud_indices_[j])
	{
	if (clouds_[i].y() <= 256)
	{
	return true;
	}
	else
	{
	std::cout << "Cloud " << i << " at following y-coordinate is offscreen: " << clouds_[i].y() << std::endl;
	return false;
	}
	}
	}

	std::cout << "Cloud " << i << " is not supposed to be onscreen." << std::endl;
	return false;
	*/
}

void CloudMachine::FixClouds()
{
	// Fix all the offscreen clouds
	for (int i = 0; i < 2 * length; i++)
	{
		if (!Onscreen(i))
		{
			//std::cout << "Resetting cloud at raw index " << i << " at y-coordinate " << clouds_[i].y() << std::endl;
			clouds_[i].Init();
		}
	}
}

void CloudMachine::BackgroundThread()
{
	while (true)
	{
		FixClouds();
	}
}

void CloudMachine::AddPreppedCloud(int n)
{
	for (int i = 0; i < length * 2; i++)
	{
		Cloud& c = clouds_[i];

		if (c.y() < 0)
		{
			bool isunique = true;

			for (int j = 0; j < length; j++)
			{
				if (cloud_indices_[j] == i)
				{
					isunique = false;
				}
			}

			if (isunique)
			{
				cloud_indices_[n] = i;
			}

			//std::cout << "Inserted cloud " << i << " at " << c.x() << std::endl;

			return;
		}
	}

	std::cout << "Cloud backup!" << std::endl;
}

CloudMachine::CloudMachine(int n)
{
	length = n;

	clouds_ = new Cloud[2 * length];

	for (int i = 0; i < 2 * length; i++)
	{
		clouds_[i].sprite_.setPosition((rand() % CLOUD_WIDTH) - (CLOUD_WIDTH / 2), (i + 2) * (-128));
	}

	cloud_indices_ = new int[length];

	for (int i = 0; i < length; i++)
	{
		cloud_indices_[i] = i;
	}

	background_ = std::thread(&CloudMachine::BackgroundThread, this);
}

void CloudMachine::Update()
{
	int n = 1;
	sf::Clock clock;

	for (int i = 0; i < length; i++)
	{
		if (clouds_[cloud_indices_[i]].y() + CLOUD_HEIGHT <= 0)
		{
			AddPreppedCloud(i);
		}

		clouds_[cloud_indices_[i]].sprite_.move(0, 1 + (3 * (++n) / (2 * length)) );

		/*
		std::cout << "(" << clouds_[cloud_indices_[i]].x() << ", " << clouds_[cloud_indices_[i]].y() << "),  onscreen clouds: ";
		for (int i = 0; i < length; i++)
		{
		std::cout << cloud_indices_[i];
		}
		std::cout << std::endl;
		*/
	}
}

void CloudMachine::Render(sf::RenderWindow& window)
{
	for (int i = 0; i < length; i++)
	{
		window.draw(clouds_[cloud_indices_[i]].sprite_);
	}
}

// ----------------------------------------------

#endif