#ifndef __WS_CLOCK_H__
#define __WS_CLOCK_H__
#include <chrono>

class wsClock {
	std::chrono::system_clock::time_point start, last;
public:
	double getPeriodSinceCreate(int updateLast) {
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		std::chrono::nanoseconds dur = (now - start);
		if (updateLast) last = now;
		return double(dur.count());
	}
	double getPeriodSinceLastCall(int updateLast) {
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		std::chrono::nanoseconds dur = (now - last);
		if (updateLast) last = now;
		return double(dur.count());
	}
	wsClock() : start(std::chrono::system_clock::now()), last(start) {}
};
#endif