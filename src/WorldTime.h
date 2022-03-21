#pragma once
#include <chrono>

class WorldTime {
private:
	std::chrono::steady_clock::time_point prev;
	unsigned int _dt;
public:
	WorldTime();
	void updateTime();
	unsigned int dt() const;
};