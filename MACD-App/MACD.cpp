#include "Macd.h"
#include <cstdio>
#include <string>


#define MIN_SAMPLE_AMOUNT 50
#define MACD_PERIOD 26
#define SMALLER_MACD_PERIOD 12 
#define SIGNAL_PERIOD 9
#define CHECKED_LAST_ELEMENTS 4 // minimum 1
#define MS_PERIOD MACD_PERIOD + SIGNAL_PERIOD

Macd::Macd()
{
	samplesAmount = 0;
	samples = NULL;
	signals = NULL;
	macdIndicators = NULL;
}

double Macd::ema(int period, double* values, double alpha)
{

	double multiplier = 1, result = 0, numerator = 0, denominator = 0, factor = 1 - alpha;
	for (int i = 0; i < period + 1; ++i) {
		numerator += multiplier * (*(values - i));
		denominator += multiplier;
		multiplier *= factor;
	}
	return numerator/denominator;
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
		fprintf(signal, "%f\n", signals[i]);
		fprintf(macd, "%f\n", macdIndicators[i]);
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
	macdIndicators = new double[samplesAmount];
	if (signals) delete[] signals;
	signals = new double[samplesAmount];
	for (int i = 0; i < MS_PERIOD; ++i) {
		if(i<MACD_PERIOD) macdIndicators[i] = 0;
		signals[i] = 0;
	}
}

bool Macd::isLastHigher(int amount, double * tab1, double * tab2)
{
	for (int i = 1; i < amount + 1; ++i) {
		if (*(tab1 - i) < *(tab2 - i)) return false; // co najmniej 1 z ostatnich elementów jest mniejszy
	}
	return true;
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
	double	wallet = 0,
			lastMacd = macdIndicators[MS_PERIOD-1],
			lastSignal = signals[MS_PERIOD-1];
	int		stockToBuy = 0,
			stock = initialStock;

	for (int i = MS_PERIOD; i < samplesAmount; ++i) {
		if (macdIndicators[i] < signals[i] &&
			isLastHigher(CHECKED_LAST_ELEMENTS, &macdIndicators[i], &signals[i])) { // MACD przecina z góry - sprzeda¿ akcji
			wallet += stock * samples[i];
			stock = 0;
		}
		else if (macdIndicators[i] > signals[i] &&
				isLastHigher(CHECKED_LAST_ELEMENTS, &signals[i], &macdIndicators[i])) { // z do³u - zakup akcji
			stockToBuy = wallet / samples[i];
			stock += stockToBuy;
			wallet -= stockToBuy * samples[i];
		}
		lastMacd = macdIndicators[i];
		lastSignal = signals[i];
	}
	return (stock*samples[samplesAmount-1] + wallet)/(initialStock* samples[0]);
}

void Macd::calculate(char * outputFile)
{
	for (int i = MACD_PERIOD; i < samplesAmount; ++i) {
		macdIndicators[i] = ema(SMALLER_MACD_PERIOD, samples + i , 2.0 / (SMALLER_MACD_PERIOD - 1.0)) -
							ema(MACD_PERIOD, samples + i, 2.0 / (MACD_PERIOD - 1.0));
	}
	for (int i = MS_PERIOD; i < samplesAmount; ++i) {
		signals[i] = ema(SIGNAL_PERIOD, macdIndicators + i, 2.0 /(SIGNAL_PERIOD - 1.0));
	}

	writeToFile(outputFile);
}


Macd::~Macd()
{
	if (samples) delete[] samples;
	if (macdIndicators) delete[] macdIndicators;
	if (signals) delete[] signals;
}
