#include <iostream>
#include "MACD.h"

using namespace std;

int main(int argc, char* argv[]) {
	//if (argc < 3) return 0;

	//char* inputFile = argv[1];
	//char* outputFile = argv[2];

	char ifile[] = "./Samples.txt";
	char ofile[] = "wynik";
	MACD macd;
	macd.loadSamples(ifile, 1000);
	macd.calculate(ofile);

	//double table[3] = { 5.0, 6.0, 7.0 };
	//double value = macd.ema(3, table + 2, 0.5);
	//macd.compute(outputFile);

	return 0;
}