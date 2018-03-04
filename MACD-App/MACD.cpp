#include "MACD.h"
#include <cstdio>


MACD::MACD(char* inputFile)
{
	FILE* input;
	int iter = 0;
	input = fopen(inputFile, "r");
	while (fscanf(input, "%lf", &samples[iter]) != EOF) {
		++iter;
	}
	samplesLength = iter;
}

double MACD::optimalEarning(int days)
{
	return 0.0;
}

void MACD::compute(char * outputFile)
{


}


MACD::~MACD()
{
}
