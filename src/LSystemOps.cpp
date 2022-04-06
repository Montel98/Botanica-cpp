#include "LSystemOps.h"
#include "StemBuilder.h"

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

LSystemStackFrame RotateHorizontal::operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) {
	LSystemStackFrame newFrame = frame;
	newFrame.lParams.axis.rotateFrameHorizontal(horizontalAngle);
	return newFrame;
}

LSystemStackFrame RotateVertical::operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) {
	LSystemStackFrame newFrame = frame;
	newFrame.lParams.axis.rotateFrameVertical(verticalAngle);
	return newFrame;
}

LSystemStackFrame RotateRoll::operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) {
	LSystemStackFrame newFrame = frame;
	newFrame.lParams.axis.rotateFrameRoll(rollAngle);
	return newFrame;
}

LSystemStackFrame RotatePlane::operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) {
	LSystemStackFrame newFrame = frame;
	newFrame.lParams.axis.rotateFrameVertical(_verticalAngle);
	newFrame.lParams.axis.rotateFrameHorizontal(_horizontalAngle);
	return newFrame;
}

LSystemStackFrame StackPush::operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) {

	StemBuilder::StemBodyRadius radiusFunc(
		frame.lParams.radiusStart,
		frame.lParams.radiusEnd,
		frame.lParams.branchLength,
		frame.lParams.count
	);

	frames.push_back(frame);
	LSystemStackFrame newFrame = frame;
	newFrame.lParams.radiusStart = 1.2f * radiusFunc(newFrame.lParams.count);
	newFrame.lParams.radiusEnd = 0.3f * radiusFunc(newFrame.lParams.branchLength - 1);
	newFrame.lParams.connectParent = _connectParent;
	newFrame.lParams.level++;
	newFrame.lParams.count = 0;
	newFrame.lParams.branchLength = LSystemOps::getNoSegmentsInRange(
		lString,
		newFrame.lParams.stringIndex + 1,
		LSystemOps::skipBranch(lString, newFrame.lParams.stringIndex) + 1
	);
	return newFrame;
}

LSystemStackFrame StackPop::operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) {
	LSystemStackFrame newFrame = frames.back();
	frames.pop_back();
	return newFrame;
}

LSystemStackFrame GenStem::operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) {
	LSystemStackFrame newFrame = frame;
	newFrame.lParams.count++;
	newFrame.lParams.position += 0.03f * glm::normalize(newFrame.lParams.axis.forward);
	newFrame.lParams.depth++;
	newFrame.lParams.connectParent = true;
	return newFrame;
}

LSystemStackFrame NoOp::operator()(const std::vector<OpCode>& lString, const LSystemStackFrame& frame, std::vector<LSystemStackFrame>& frames) { 
	return frame;
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


namespace LSystemOps {

	int getNoSegmentsInRange(const std::vector<OpCode>& lString, int startIndex, int endIndex) {

		int segmentCount = 0;

		for (int index = startIndex; index < endIndex; index++) {

			if (lString[index].symbol == '[') {
				index = LSystemOps::skipBranch(lString, index);
				continue;
			}

			if (lString[index].symbol == '0') {
				segmentCount++;
			}
		}

		return segmentCount;
	}

	int skipBranch(const std::vector<OpCode>& lString, int index) {

		int parenthesisCount = 1;

		while (parenthesisCount != 0 && index + 1 < lString.size()) {
			index++;

			if (lString[index].symbol == '[') {
				parenthesisCount++;
			}
			else if (lString[index].symbol == ']') {
				parenthesisCount--;
			}
		}

		return index;
	}
}