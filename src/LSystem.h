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

class RotatePlane : public LSystemOp {
private:
	float _horizontalAngle, _verticalAngle;
public:
	RotatePlane(float horizontalAngle, float verticalAngle);
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

class NoOp : public LSystemOp {
public:
	NoOp() = default;
	LSystemParams operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) override;
};

struct OpCode {
	char symbol;
	std::unique_ptr<LSystemOp> params;
};

OpCode ROT_H(float angle);

OpCode ROT_V(float angle);

OpCode ROT_R(float angle);

OpCode ROT_P(float horizontalAngle, float verticalAngle);

OpCode S_PUSH(bool connectParent);

OpCode S_POP();

OpCode GEN_STEM();

OpCode BRANCH_1();

OpCode BRANCH_2();

void makeLString(std::vector<OpCode> startString);