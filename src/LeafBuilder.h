#include "FourierSeries.h"
#include <glm/glm.hpp>

class LeafSurface {
private:
	float foldMag, foldFreq, vMax;
	FourierSeries profileXY;
public:
	LeafSurface(const FourierSeries& fourier, int foldFactor, int foldFrequency, int vMax);
	glm::vec3 operator()(float u, float v) const;
	virtual float profileXZ(float x) const = 0;
};


class LeafProfileStart : public LeafSurface {
public:
	LeafProfileStart(const FourierSeries& fourier, int foldFactor, int foldFrequency, int vMax);
	float profileXZ(float x) const;
};

class LeafProfileEnd : public LeafSurface {
public:
	LeafProfileEnd(const FourierSeries& fourier, int foldFactor, int foldFrequency, int vMax);
	float profileXZ(float x) const;
};