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


FourierSeries::FourierSeries(float a0, const std::vector<FourierTerm> &t) : a0(a0), terms(t) {};

float FourierSeries::sum(float x) const {
	float sum = a0;

	for (const FourierTerm& term : terms) {
		sum += term.eval(x);
	}

	return sum;
}

FourierTerm::FourierTerm(FourierTerm &&other) noexcept : sinMagnitude(std::move(other.sinMagnitude)), 
														cosMagnitude(std::move(other.cosMagnitude)), 
														angularFreq(std::move(other.angularFreq)), 
														attenuation(std::move(other.attenuation)) { std::cout << "move being called!\n";};

FourierTerm::FourierTerm(const FourierTerm &other) noexcept : sinMagnitude(other.sinMagnitude), 
														cosMagnitude(other.cosMagnitude), 
														angularFreq(other.angularFreq), 
														attenuation(other.attenuation) { std::cout << "copy being done!\n";};