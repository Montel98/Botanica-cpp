#include "Object3D.h"
#include "Mesh.h"
#include "BezierLinear.h"
#include "glm/glm.hpp"
#include "ParametricGeometry.h"
#include "StemBuilder.h"

/*struct Axis {
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 left;
};

class StemRadius {
private:
	float radiusStart, radiusEnd;
	int noStems;
	int _shift;
	int decayRate;
protected:
	float baseRadius(float u) const;
public:
	StemRadius(float rStart, float rEnd, int branchLength, int shift);
	virtual float operator()(float u) const = 0;
	virtual StemRadius* clone() const = 0;
};

class StemBodyRadius : public StemRadius {
public:
	StemBodyRadius(float rStart, float rEnd, int branchLength, int shift);
	float operator()(float u) const override;
	StemBodyRadius* clone() const override;
};

class StemTipRadiusStart : public StemRadius {
public:
	StemTipRadiusStart(float rStart, float rEnd, int branchLength, int shift);
	float operator()(float u) const override;
	StemTipRadiusStart* clone() const override;
};

class StemTipRadiusEnd : public StemRadius {
public:
	StemTipRadiusEnd(float rStart, float rEnd, int branchLength, int shift);
	float operator()(float u) const override;
	StemTipRadiusEnd* clone() const override;
};*/

/*class StemSurface {
private:
	std::unique_ptr<StemRadius> _radiusFunc;
	BezierLinear _path;
	Axis _axis;
	glm::vec3 crossSection(float v) const;
public:
	StemSurface(std::unique_ptr<StemRadius> radiusFunc, const BezierLinear& path, const Axis& axis);
	StemSurface(const StemSurface& other);
	glm::vec3 operator()(float u, float v) const;
};

struct LSystemParams {
	glm::vec3 position;
	Axis axis;
	int count;
	int depth;
	bool connectParent;
	int stringIndex;
	float radiusStart;
	float radiusEnd;
};*/

class Stem {
public:
	Object3D worldObject;
	StemBuilder::LSystemParams lParams;
	Stem();
	Mesh generateMesh(Stem* prevStem);
	void generateGeometry(const StemBuilder::LSystemParams &lParams, Stem* prevStem);
};

/*class StemSurfaceMorphInfo {
public:
	std::unique_ptr<StemRadius> radiusStart;
	std::unique_ptr<StemRadius> radiusEnd;
	BezierLinear pathStart;
	BezierLinear pathEnd;

	StemSurfaceMorphInfo(
		std::unique_ptr<StemRadius> rStart, 
		std::unique_ptr<StemRadius> rEnd, 
		BezierLinear pathBegin, 
		BezierLinear pathFinish
	);

	StemSurfaceMorphInfo(const StemSurfaceMorphInfo& other);
};*/

/*namespace StemBuilder {

	ParametricGeometry<StemSurface> generateStemTip(const LSystemParams& lParams);
	ParametricGeometry<StemSurface> generateStemBody(const LSystemParams& lParams);

	ParametricGeometry<StemSurface> generateStemGeometry(
		StemSurfaceMorphInfo keyFrameInfo, 
		const Axis& axis,
		const GeometryConstraints& constraints
	);
}*/

//genStemPartGeometry()