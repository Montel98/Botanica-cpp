#pragma once

#include <vector>
#include <math.h>
#include <iostream>

class FourierTerm {
private:
	float sinMagnitude, cosMagnitude;
	float angularFreq;
	float attenuation;
public:
	FourierTerm(float sinMag, float cosMag, float n, float sharpness);
	FourierTerm(FourierTerm&& other) noexcept;
	FourierTerm(const FourierTerm& other) noexcept;
	float eval(float x) const;
};

class FourierSeries {
private:
	float a0;
	std::vector<FourierTerm> terms;
public:
	FourierSeries(float a0, const std::vector<FourierTerm> &t);
	float sum(float x) const;
};