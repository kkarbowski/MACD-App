#include <iostream>
#include <stdio.h>
#include "Macd.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 4) return 0;

	char* inputFile = argv[1];
	char* outputFile = argv[2];
	int samplesAmount = atoi(argv[3]);

	Macd macd;
	macd.loadSamples(inputFile, samplesAmount);
	macd.calculate(outputFile);
	cout << "Zarobek po miesiacu (poczatkowe 1000 akcji): " << macd.optimalEarning(samplesAmount);
	//getchar();

	return 0;
}