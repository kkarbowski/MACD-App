#include "Macd.h"
#include <cstdio>
#include <string>


#define MIN_SAMPLE_AMOUNT 50
#define MACD_PERIOD 26
#define SMALLER_MACD_PERIOD 12 
#define SIGNAL_PERIOD 9

Macd::Macd()
{
	samplesAmount = 0;
	samples = NULL;
	signals = NULL;
	macdIndicators = NULL;
}

double Macd::ema(int period, double* values, double alpha)
{
	if (period == 1) return *values;
	else return alpha * (*(values - period + 1)) + (1 - alpha) * ema(period - 1, values, alpha);
	return 0;
}

bool Macd::writeToFile(char* fileName)
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

void Macd::allocate()
{
	if (samples) delete[] samples;
	samples = new double[samplesAmount];

	if (macdIndicators) delete[] macdIndicators;
	macdIndicators = new double[samplesAmount - MACD_PERIOD];

	if (signals) delete[] signals;
	signals = new double[samplesAmount - MACD_PERIOD - SIGNAL_PERIOD];
}


 int Macd::loadSamples(char* inputFile, int lines)
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
	samplesAmount = iter;
	fclose(input);
	return samplesAmount;
}

double Macd::optimalEarning(int initialStock)
{
	double	* shiftedMacdInd = macdIndicators + SIGNAL_PERIOD,
			* shiftedSamples = samples + SIGNAL_PERIOD + MACD_PERIOD;
	double	wallet = 0,
			lastMacd = shiftedMacdInd[0],
			lastSignal = signals[0];
	int		stockToBuy = 0,
			stock = initialStock;

	for (int i = 1; i < samplesAmount - MACD_PERIOD  - SIGNAL_PERIOD; ++i) {
		if (lastMacd > lastSignal && shiftedMacdInd[i] < signals[i]) { // MACD przecina z góry - sprzeda¿ akcji
			wallet += stock * shiftedSamples[i];
			stock = 0;
		}
		else if (lastMacd < lastSignal && shiftedMacdInd[i] > signals[i]) { // z do³u - zakup akcji
			stockToBuy = wallet / shiftedSamples[i];
			stock += stockToBuy;
			wallet -= stockToBuy * shiftedSamples[i];
		}
		lastMacd = shiftedMacdInd[i];
		lastSignal = signals[i];
	}
	return (stock*samples[samplesAmount-1] + wallet)/(initialStock* samples[0]);
}

void Macd::calculate(char * outputFile)
{
	int totalPeriod = MACD_PERIOD + SIGNAL_PERIOD;
	for (int i = 0; i < samplesAmount - MACD_PERIOD; ++i) {
		macdIndicators[i] = ema(SMALLER_MACD_PERIOD, samples + i + MACD_PERIOD - 1, 2 / (SMALLER_MACD_PERIOD - 1.0)) - ema(MACD_PERIOD, samples + i + MACD_PERIOD - 1, 2 / (MACD_PERIOD - 1.0));
	}
	for (int i = 0; i < samplesAmount - totalPeriod; ++i) {
		signals[i] = ema(SIGNAL_PERIOD, macdIndicators + i + SIGNAL_PERIOD - 1, 2/(SIGNAL_PERIOD - 1.0));
	}

	writeToFile(outputFile);
}


Macd::~Macd()
{
	if (samples) delete[] samples;
	if (macdIndicators) delete[] macdIndicators;
	if (signals) delete[] signals;
}
