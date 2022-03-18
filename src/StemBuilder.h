#pragma once

#include "BezierLinear.h"
#include "glm/glm.hpp"
#include "ParametricGeometry.h"
#include "Axis.h"
#include "LSystem.h"

namespace StemBuilder {

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
	};

	class StemSurface {
	private:
		std::unique_ptr<StemRadius> _radiusFunc;
		BezierLinear _path;
		Axis _axis;
		glm::vec3 crossSection(float u, float v) const;
	public:
		StemSurface(std::unique_ptr<StemRadius> radiusFunc, const BezierLinear& path, const Axis& axis);
		StemSurface(const StemSurface& other);
		glm::vec3 operator()(float u, float v) const;
	};

	class StemSurfaceMorphInfo {
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
	};

	ParametricGeometry<StemSurface> generateStemTip(
		const LSystemParams& lParams, 
		const GeometryConstraints& constraints
	);

	ParametricGeometry<StemSurface> generateStemBody(
		const LSystemParams& lParams, 
		const GeometryConstraints& constraints
	);

	ParametricGeometry<StemSurface> generateStemGeometry(
		const StemSurfaceMorphInfo& keyFrameInfo, 
		const Axis& axis,
		const GeometryConstraints& constraints
	);
}