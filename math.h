// ----------------------------------------------
//			MATH.H
// ----------------------------------------------
// Selection of utility functions.
// ---------------------------------------------- 

#define M_PI           3.14159265358979323846

// ---------------------------------------------- 

#ifndef SRC_MATH
#define SRC_MATH

int RollStat()
{
	return (1 + rand() % 6) + (1 + rand() % 6) + (1 + rand() % 6);
}

float RandDouble()
{
	return static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
}

int Exp(int x, int y)
{
	int ret = 1;

	for (int i = 0; i < y; i++)
	{
		ret *= x;
	}

	return ret;
}

#endif