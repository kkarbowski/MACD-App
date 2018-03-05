#pragma once

class MACD
{
private:
	double* samples;
	double* macdIndicators;
	double* signals;
	int samplesAmount;

	
	bool writeToFile(char* fileName);
	void allocate();
public:
	MACD();
	double ema(int period, double* values, double alpha);
	int loadSamples(char* inputFile, int lines);
	double optimalEarning(int days);
	void calculate(char* outputFile);
	~MACD();
};

