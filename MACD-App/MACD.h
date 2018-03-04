#pragma once
#define MAX_SAMPLES 1000
class MACD
{
private:
	double samples[MAX_SAMPLES];
	int samplesLength;
	char* inputFileName;
	char* outputFileName;
public:
	MACD(char* inputFile);
	double optimalEarning(int days);
	void compute(char* outputFile);
	~MACD();
};

