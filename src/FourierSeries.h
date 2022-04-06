#pragma once

#include <vector>
#include <cmath>
#include <iostream>

class FourierTerm {
private:
	float sinMagnitude, cosMagnitude;
	float angularFreq;
	float attenuation;
public:
	FourierTerm(float sinMag, float cosMag, float n, float sharpness);
	float eval(float x) const;
};

class FourierSeries {
private:
	float a0;
	std::vector<FourierTerm> terms;
public:
	FourierSeries(float a0, const std::vector<FourierTerm>& fourierTerms);
	float sum(float x) const;
};