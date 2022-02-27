#include "LSystemOps.h"

RotateHorizontal::RotateHorizontal(float angle) : horizontalAngle(angle) {};
RotateVertical::RotateVertical(float angle) : verticalAngle(angle) {};
RotateRoll::RotateRoll(float angle) : rollAngle(angle) {};
RotatePlane::RotatePlane(float horizontalAngle, float verticalAngle) :
 _horizontalAngle(horizontalAngle), _verticalAngle(verticalAngle) {};

RotateHorizontal* RotateHorizontal::clone() const {
	return new RotateHorizontal(*this);
}

RotateVertical* RotateVertical::clone() const {
	return new RotateVertical(*this);
}

RotateRoll* RotateRoll::clone() const {
	return new RotateRoll(*this);
}

RotatePlane* RotatePlane::clone() const {
	return new RotatePlane(*this);
}

StackPush* StackPush::clone() const {
	return new StackPush(*this);
}

StackPop* StackPop::clone() const {
	return new StackPop(*this);
}

GenStem* GenStem::clone() const {
	return new GenStem(*this);
}

NoOp* NoOp::clone() const {
	return new NoOp(*this);
}

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

OpCode::OpCode(char opSymbol, std::unique_ptr<LSystemOp> opFunc) : 
params(std::move(opFunc)), symbol(opSymbol) {}

OpCode::OpCode(const OpCode& other) 
: params(std::unique_ptr<LSystemOp>(other.params->clone())), symbol(other.symbol) {};

OpCode& OpCode::operator=(const OpCode& other) {
	params = std::unique_ptr<LSystemOp>(other.params->clone());
	symbol = other.symbol;
	return *this;
}

OpCode ROT_H(float angle) {
	return OpCode('H', std::make_unique<RotateHorizontal>(angle));
}

OpCode ROT_V(float angle) {
	return OpCode('V', std::make_unique<RotateVertical>(angle));
}

OpCode ROT_R(float angle) {
	return OpCode('R', std::make_unique<RotateRoll>(angle));
}

OpCode ROT_P(float horizontalAngle, float verticalAngle) {
	return OpCode('*', std::make_unique<RotatePlane>(horizontalAngle, verticalAngle));
}

OpCode S_PUSH(bool connectParent) {
	return OpCode('[', std::make_unique<StackPush>(connectParent));
}

OpCode S_POP() {
	return OpCode(']', std::make_unique<StackPop>());
}

OpCode GEN_STEM() {
	return OpCode('0', std::make_unique<GenStem>());
}

OpCode BRANCH_1() {
	return OpCode('1', std::make_unique<NoOp>());
}

OpCode BRANCH_2() {
	return OpCode('2', std::make_unique<NoOp>());
}