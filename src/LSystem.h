#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Axis.h"

struct LSystemParams {
		glm::vec3 position;
		Axis axis;
		int count;
		int depth;
		bool connectParent;
		int stringIndex;
		float radiusStart;
		float radiusEnd;
};

class LSystemOp {

	virtual LSystemParams operator()(const LSystemParams &lParams, std::vector<LSystemParams>& lParamsStack) = 0;
};

class RotateHorizontal : public LSystemOp {
private:
	float horizontalAngle;
public:
	RotateHorizontal(float angle);
	LSystemParams operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) override;
};

class RotateVertical : public LSystemOp {
private:
	float verticalAngle;
public:
	RotateVertical(float angle);
	LSystemParams operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) override;
};

class RotateRoll : public LSystemOp {
private:
	float rollAngle;
public:
	RotateRoll(float angle);
	LSystemParams operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) override;
};

class StackPush : public LSystemOp {
private:
	bool _connectParent;
public:
	StackPush(bool connectParent);
	LSystemParams operator()(const LSystemParams& params, std::vector<LSystemParams>& lParamsStack) override;
};

class StackPop : public LSystemOp {
public:
	StackPop() = default;
	LSystemParams operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) override;
};

class GenStem : public LSystemOp {
public:
	GenStem() = default;
	LSystemParams operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) override;
};

std::unique_ptr<RotateHorizontal> ROT_H(float angle);

std::unique_ptr<RotateVertical> ROT_V(float angle);

std::unique_ptr<RotateRoll> ROT_R(float angle);

std::unique_ptr<StackPush> S_PUSH(bool connectParent);

std::unique_ptr<StackPop> S_POP();

std::unique_ptr<GenStem> GEN_STEM();

void makeLString();