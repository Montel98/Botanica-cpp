#include "WorldTime.h"

WorldTime::WorldTime() : prev(std::chrono::steady_clock::now()) {
}

void WorldTime::updateTime() {
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	_dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - prev).count();
	prev = now;
}

unsigned int WorldTime::dt() const {
	return _dt;
}