#include "MACD.h"
#include <cstdio>
#include <string>


#define MIN_SAMPLE_AMOUNT 50
#define MACD_PERIOD 26
#define MACD_PERIOD_1 12 // < MACD_PERIOD
#define SIGNAL_PERIOD 9

MACD::MACD()
{
	samplesAmount = 0;
	samples = NULL;
	signals = NULL;
	macdIndicators = NULL;
}

double MACD::ema(int period, double* values, double alpha)
{
	if (period == 1) return *values;
	else return alpha * (*(values - period + 1)) + (1 - alpha) * ema(period - 1, values, alpha);
	return 0;
}

bool MACD::writeToFile(char* fileName)
{
	FILE* signal,* macd;
	std::string name(fileName);
	signal = fopen((name + ".signal").c_str(), "w");
	if (signal == NULL) return false;
	macd = fopen((name + ".macd").c_str(), "w");
	if (macd == NULL) {
		fclose(signal);
		return false;
	}
	for (int i = 0; i < samplesAmount; ++i) {
		if (i < SIGNAL_PERIOD + MACD_PERIOD) fprintf(signal, "0\n");
		else fprintf(signal, "%f\n", signals[i - SIGNAL_PERIOD - MACD_PERIOD]);
		if (i < MACD_PERIOD) fprintf(macd, "0\n");
		else fprintf(macd, "%f\n", macdIndicators[i - MACD_PERIOD]);
	}

	fclose(macd);
	fclose(signal);
	return true;
}

void MACD::allocate()
{
	if (samples) delete[] samples;
	samples = new double[samplesAmount];

	if (macdIndicators) delete[] macdIndicators;
	macdIndicators = new double[samplesAmount - MACD_PERIOD];

	if (signals) delete[] signals;
	signals = new double[samplesAmount - MACD_PERIOD - SIGNAL_PERIOD];
}


 int MACD::loadSamples(char* inputFile, int lines)
{
	FILE* input;
	int iter = 0;
	if (lines < MIN_SAMPLE_AMOUNT) return 0;
	samplesAmount = lines;
	input = fopen(inputFile, "r");
	if (input == NULL) return 0;
	allocate();

	while (fscanf(input, "%lf", &samples[iter]) != EOF && iter < samplesAmount) {
		++iter;
	}
	fclose(input);
	return samplesAmount;
}

double MACD::optimalEarning(int days)
{
	return 0.0;
}

void MACD::calculate(char * outputFile)
{
	int totalPeriod = MACD_PERIOD + SIGNAL_PERIOD;
	for (int i = 0; i < samplesAmount - MACD_PERIOD; ++i) {
		macdIndicators[i] = ema(MACD_PERIOD_1, samples + i + MACD_PERIOD - 1, 2 / (MACD_PERIOD_1 - 1.0)) - ema(MACD_PERIOD, samples + i + MACD_PERIOD - 1, 2 / (MACD_PERIOD - 1.0));
	}
	for (int i = 0; i < samplesAmount - totalPeriod; ++i) {
		signals[i] = ema(SIGNAL_PERIOD, macdIndicators + i + SIGNAL_PERIOD - 1, 2/(SIGNAL_PERIOD - 1.0));
	}

	writeToFile(outputFile);
}


MACD::~MACD()
{
	if (samples) delete[] samples;
	if (macdIndicators) delete[] macdIndicators;
	if (signals) delete[] signals;
}
