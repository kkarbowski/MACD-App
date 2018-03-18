#pragma once

class Macd
{
private:
	double* samples;
	double* macdIndicators;
	double* signals;
	int samplesAmount;


	bool writeToFile(char* fileName);
	void allocate();
public:
	Macd();
	double ema(int period, double* values, double alpha);
	int loadSamples(char* inputFile, int lines);
	double optimalEarning(int initialStock);
	void calculate(char* outputFile);
	~Macd();
};

