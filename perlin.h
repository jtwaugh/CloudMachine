// ----------------------------------------------
//			PERLIN.H
// ----------------------------------------------
// Generates "Perlin" noise
// ---------------------------------------------- 

#ifndef SRC_PERLIN
#define SRC_PERLIN


// ---------------------------------------------- 

#include <cmath>
#include "math.h"

// ---------------------------------------------- 

double Interpolate(float x0, float x1, float alpha)
{
	return x0 * (1 - alpha) + alpha * x1;
}

double* WhiteNoise(int width, int height)
{
	double* noise = new double[width * height];

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			noise[i * width + j] = RandDouble();
		}
	}

	return noise;
}

double* SmoothNoise(int width, int height, int k)
{
	double* base = WhiteNoise(width, height);

	double* noise = new double[width * height];

	int samplePeriod = Exp(2, k);
	double sampleFrequency = 1.0 / samplePeriod;

	for (int i = 0; i < width; i++)
	{
		int sample_i0 = (i / samplePeriod) * samplePeriod;
		int sample_i1 = (sample_i0 + samplePeriod) % width;
		double horizontal_blend = (i - sample_i0) * sampleFrequency;

		for (int j = 0; j < height; j++)
		{
			int sample_j0 = (j / samplePeriod) * samplePeriod;
			int sample_j1 = (sample_j0 + samplePeriod) % height;
			double vertical_blend = (j - sample_j0) * sampleFrequency;

			double top = Interpolate(base[sample_i0 + width * sample_j0], base[sample_i1 + width * sample_j0], horizontal_blend);
			double bottom = Interpolate(base[sample_i0 + width * sample_j1], base[sample_i1 + width * sample_j1], horizontal_blend);

			noise[i + width * j] = Interpolate(top, bottom, vertical_blend);
		}
	}

	return noise;
}

double* PerlinNoise(int width = 256, int height = 256, int k = 8, double amplitude = 1.0, double persistence = 0.5)
{
	double total = 0.0;

	double** smoothNoise = new double*[k];

	for (int i = 0; i < k; i++)
	{
		smoothNoise[i] = SmoothNoise(width, height, i);
	}

	double* perlin = new double[width * height];

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			perlin[i + width * j] = 0;
		}
	}

	for (int q = (k - 1); q > 0; q--)
	{
		amplitude *= persistence;
		total += amplitude;

		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				perlin[i + width * j] += smoothNoise[q][i + width * j] * amplitude;
			}
		}
	}

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			perlin[i + width * j] /= total;
		}
	}

	return perlin;
}

double* WoodNoise(int width = 256, int height = 256, int k = 8, double amplitude = 1.0, double persistence = 0.5)
{
	double* perlin = PerlinNoise(width, height, k, amplitude, persistence);

	double normalizer = 1.0 / (M_PI * 2.0);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			double dx = i - (width / 2);
			double dy = j - (height / 2);

			double distance = sqrt(dx * dx + dy * dy);
			
			double gauss = normalizer * distance;

			perlin[i + width * j] *= gauss;
			if (perlin[i + width * j] < 0)
			{
				perlin[i + width * j] = 0;
			}
		}
	}

	return perlin;
}

double* NoiseCloud(int width = 256, int height = 256, int k = 8, double amplitude = 1.0, double persistence = 0.5)
{
	double* perlin = PerlinNoise(width, height, k, amplitude, persistence);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			double _width = (double)width / 2;
			double _height = (double)height / 2;

			double dx = (i - ((double)width / 2)) / _width;
			double dy = (j - ((double)height / 2)) / _height;

			double distance = dx * dx + dy * dy;

			double gauss = 0;

			if (1 - (dx * dx) - (dy * dy) > 0)
			{
				gauss = sqrt(1 - (dx * dx) - (dy * dy));
			}

			perlin[i + width * j] *= gauss;

			if (perlin[i + width * j] < 0.4)
			{
				perlin[i + width * j] = 0;
			}
		}
	}

	return perlin;
}

// ---------------------------------------------- 

#endif