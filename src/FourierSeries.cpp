#include "FourierSeries.h"


FourierTerm::FourierTerm(float sinMag, float cosMag, float n, float sharpness) : sinMagnitude(sinMag), 
																	cosMagnitude(cosMag), 
																	angularFreq(n),
																	attenuation(sharpness) {};


float FourierTerm::eval(float x) const {
	float cosComponent = cosMagnitude * pow((cos(angularFreq * x)), attenuation);
	float sinComponent = sinMagnitude * pow((sin(angularFreq * x)), attenuation);
	return cosComponent + sinComponent;
}


FourierSeries::FourierSeries(float a0, const std::vector<FourierTerm>& fourierTerms) : a0(a0), terms(fourierTerms) {};

float FourierSeries::sum(float x) const {
	float sum = a0;

	for (const FourierTerm& term : terms) {
		sum += term.eval(x);
	}

	return sum;
}