#include "LSystem.h"

RotateHorizontal::RotateHorizontal(float angle) : horizontalAngle(angle) {};
RotateVertical::RotateVertical(float angle) : verticalAngle(angle) {};
RotateRoll::RotateRoll(float angle) : rollAngle(angle) {};
RotatePlane::RotatePlane(float horizontalAngle, float verticalAngle) :
 _horizontalAngle(horizontalAngle), _verticalAngle(verticalAngle) {};

StackPush::StackPush(bool connectParent) : _connectParent(connectParent) {};

LSystemParams RotateHorizontal::operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) {
	LSystemParams newLParams = lParams;
	newLParams.axis.rotateFrameHorizontal(horizontalAngle);
	return lParams;
}

LSystemParams RotateVertical::operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) {
	LSystemParams newLParams = lParams;
	newLParams.axis.rotateFrameVertical(verticalAngle);
	return lParams;
}

LSystemParams RotateRoll::operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) {
	LSystemParams newLParams = lParams;
	newLParams.axis.rotateFrameRoll(rollAngle);
	return lParams;
}

LSystemParams RotatePlane::operator()(const LSystemParams &lParams, std::vector<LSystemParams>& lParamsStack) {
	LSystemParams newLParams = lParams;
	newLParams.axis.rotateFrameHorizontal(_horizontalAngle);
	newLParams.axis.rotateFrameVertical(_verticalAngle);
	return lParams;
}

LSystemParams StackPush::operator()(const LSystemParams& params, std::vector<LSystemParams>& lParamsStack) {
	lParamsStack.push_back(params);
	return params;
}

LSystemParams StackPop::operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) {
	LSystemParams newLParams = lParamsStack.back();
	lParamsStack.pop_back();
	return newLParams;
}

LSystemParams GenStem::operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) {
	LSystemParams newLParams = lParams;
	newLParams.count++;
	newLParams.position = lParams.position + 0.03f * lParams.axis.forward;
	newLParams.depth++;
	newLParams.connectParent = true;

	return newLParams;
}

LSystemParams NoOp::operator()(const LSystemParams& lParams, std::vector<LSystemParams>& lParamsStack) { 
	return lParams;
}


OpCode ROT_H(float angle) {
	return {'H', std::make_unique<RotateHorizontal>(angle)};
}

OpCode ROT_V(float angle) {
	return {'V', std::make_unique<RotateVertical>(angle)};
}

OpCode ROT_R(float angle) {
	return {'R', std::make_unique<RotateRoll>(angle)};
}

OpCode ROT_P(float horizontalAngle, float verticalAngle) {
	return {'*', std::make_unique<RotatePlane>(horizontalAngle, verticalAngle)};
}

OpCode S_PUSH(bool connectParent) {
	return {'[', std::make_unique<StackPush>(connectParent)};
}

OpCode S_POP() {
	return {']', std::make_unique<StackPop>()};
}

OpCode GEN_STEM() {
	return {'0', std::make_unique<GenStem>()};
}

OpCode BRANCH_1() {
	return {'1', std::make_unique<NoOp>()};
}

OpCode BRANCH_2() {
	return {'2', std::make_unique<NoOp>()};
}

void makeLString(std::vector<OpCode> startString) {
	std::vector<OpCode> lString;
	lString.push_back(GEN_STEM());
	lString.push_back(ROT_V(0.4f));
}