#pragma once

#include <chrono>
#include <exception>
#include <string>
#include <ratio>

namespace Time
{
	typedef std::chrono::time_point<std::chrono::high_resolution_clock> h_res_clock_t;

	class TimeMeasure
	{
	public:
		TimeMeasure() : start_was_called(false) {};
		~TimeMeasure() {};
		void StartMeasure();
		void StopMeasure();
		std::chrono::seconds GetDifference() const;
	private:
		bool start_was_called;
		h_res_clock_t start_time;
		h_res_clock_t end_time;
	};
}