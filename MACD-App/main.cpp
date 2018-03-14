#include <iostream>
#include <stdio.h>
#include "Macd.h"

using namespace std;

int main(int argc, char* argv[]) {
	char onlyCalculate[1] = {0}, stdOut[] = "out";
	char* outputFile = stdOut;
	char* inputFile = NULL;
	int samplesAmount = 1000;

	switch (argc) {
	case 1:
		return 0;
	case 3:
		samplesAmount = atoi(argv[2]);
		outputFile = onlyCalculate;
		break;
	case 4:
		outputFile = argv[2];
		samplesAmount = atoi(argv[3]);
		break;
	}
	inputFile = argv[1];

	Macd macd;
	macd.loadSamples(inputFile, samplesAmount);
	macd.calculate(outputFile);
	cout << "Zarobek po miesiacu (poczatkowe 1000 akcji): " << macd.optimalEarning(samplesAmount);

	return 0;
}