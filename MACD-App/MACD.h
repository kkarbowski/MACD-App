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
	bool isLastHigher(int amount, double * tab1, double * tab2);
public:
	Macd();
	double ema(int period, double* values, double alpha);
	int loadSamples(char* inputFile, int lines);
	double optimalEarning(int initialStock);
	void calculate(char* outputFile);
	~Macd();
};

