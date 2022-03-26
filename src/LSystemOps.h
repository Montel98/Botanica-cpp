#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Axis.h"
#include "LSystemParams.h"

class OpCode;

class LSystemOp {
public:
	virtual LSystemStackFrame operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) = 0;
	virtual LSystemOp* clone() const = 0;
};

class RotateHorizontal : public LSystemOp {
private:
	float horizontalAngle;
public:
	RotateHorizontal(float angle);
	LSystemStackFrame operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) override;
	RotateHorizontal* clone() const override;
};

class RotateVertical : public LSystemOp {
private:
	float verticalAngle;
public:
	RotateVertical(float angle);
	LSystemStackFrame operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) override;
	RotateVertical* clone() const override;
};

class RotateRoll : public LSystemOp {
private:
	float rollAngle;
public:
	RotateRoll(float angle);
	LSystemStackFrame operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) override;
	RotateRoll* clone() const override;
};

class RotatePlane : public LSystemOp {
private:
	float _horizontalAngle, _verticalAngle;
public:
	RotatePlane(float horizontalAngle, float verticalAngle);
	LSystemStackFrame operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) override;
	RotatePlane* clone() const override;
};

class StackPush : public LSystemOp {
private:
	bool _connectParent;
public:
	StackPush(bool connectParent);
	LSystemStackFrame operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) override;
	StackPush* clone() const override;
};

class StackPop : public LSystemOp {
public:
	StackPop() = default;
	LSystemStackFrame operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) override;
	StackPop* clone() const override;
};

class GenStem : public LSystemOp {
public:
	GenStem() = default;
	LSystemStackFrame operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) override;
	GenStem* clone() const override;
};

class NoOp : public LSystemOp {
public:
	NoOp() = default;
	LSystemStackFrame operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) override;
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

namespace LSystemOps {
	int skipBranch(const std::vector<OpCode>& lString, int index);
	int getNoSegmentsInRange(const std::vector<OpCode>& lString, int startIndex, int endIndex);
}