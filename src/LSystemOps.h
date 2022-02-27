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
public:
	virtual LSystemParams operator()(const LSystemParams &lParams, std::vector<LSystemParams>& lParamsStack) = 0;
	virtual LSystemOp* clone() const = 0;
};

class RotateHorizontal : public LSystemOp {
private:
	float horizontalAngle;
public:
	RotateHorizontal(float angle);
	LSystemParams operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) override;
	RotateHorizontal* clone() const override;
};

class RotateVertical : public LSystemOp {
private:
	float verticalAngle;
public:
	RotateVertical(float angle);
	LSystemParams operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) override;
	RotateVertical* clone() const override;
};

class RotateRoll : public LSystemOp {
private:
	float rollAngle;
public:
	RotateRoll(float angle);
	LSystemParams operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) override;
	RotateRoll* clone() const override;
};

class RotatePlane : public LSystemOp {
private:
	float _horizontalAngle, _verticalAngle;
public:
	RotatePlane(float horizontalAngle, float verticalAngle);
	LSystemParams operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) override;
	RotatePlane* clone() const override;
};

class StackPush : public LSystemOp {
private:
	bool _connectParent;
public:
	StackPush(bool connectParent);
	LSystemParams operator()(const LSystemParams& params, std::vector<LSystemParams>& lParamsStack) override;
	StackPush* clone() const override;
};

class StackPop : public LSystemOp {
public:
	StackPop() = default;
	LSystemParams operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) override;
	StackPop* clone() const override;
};

class GenStem : public LSystemOp {
public:
	GenStem() = default;
	LSystemParams operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) override;
	GenStem* clone() const override;
};

class NoOp : public LSystemOp {
public:
	NoOp() = default;
	LSystemParams operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) override;
	NoOp* clone() const override;
};

class OpCode {
public:
	char symbol;
	std::unique_ptr<LSystemOp> params;
	OpCode(char opSymbol, std::unique_ptr<LSystemOp> opFunc);
	OpCode(const OpCode& other);
	OpCode& operator=(const OpCode& other);
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