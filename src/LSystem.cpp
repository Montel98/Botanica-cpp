#include "LSystem.h"

RotateHorizontal::RotateHorizontal(float angle) : horizontalAngle(angle) {};
RotateVertical::RotateVertical(float angle) : verticalAngle(angle) {};
RotateRoll::RotateRoll(float angle) : rollAngle(angle) {};
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


std::unique_ptr<RotateHorizontal> ROT_H(float angle) {
	return std::make_unique<RotateHorizontal>(angle);
}

std::unique_ptr<RotateVertical> ROT_V(float angle) {
	return std::make_unique<RotateVertical>(angle);
}

std::unique_ptr<RotateRoll> ROT_R(float angle) {
	return std::make_unique<RotateRoll>(angle);
}

std::unique_ptr<StackPush> S_PUSH(bool connectParent) {
	return std::make_unique<StackPush>(connectParent);
}

std::unique_ptr<StackPop> S_POP() {
	return std::make_unique<StackPop>();
}

std::unique_ptr<GenStem> GEN_STEM() {
	return std::make_unique<GenStem>();
}