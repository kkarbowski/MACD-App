#include <iostream>
#include "MACD.h"

using namespace std;

int main(int argc, char* argv[]) {
	//if (argc < 3) return 0;

	//char* inputFile = argv[1];
	char inputFile[] = "./Samples.txt";
	char* outputFile = argv[2];
	MACD macd(inputFile);
	macd.compute(outputFile);


}