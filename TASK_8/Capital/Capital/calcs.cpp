#include "header.h"

int K;

vector<double> gains;
vector<double> price;

void init()
{
	srand(static_cast<unsigned int>(time(nullptr)));
	K = rand() % 78 + 3;
}

void pricecheck()
{
	double total = 0.0;
	for (int i = 0; i < N; i++)
	{
		total += price[i];
	}

	if (total > C)
	{
		cout << "\nYou don't have enough money for these! Please, enter the correct values.\n";
		getinput_fix();
	}
}

double calcs()
{
	int R = 0.0;
	for (int i = 0; i < N; i++)
	{
		R += gains[i];
	}
	return R;
}
