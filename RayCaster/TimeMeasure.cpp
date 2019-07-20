#include "TimeMeasure.h"

void Time::TimeMeasure::StartMeasure()
{
	if (!this->start_was_called)
	{
		this->start_was_called = true;
		this->start_time = std::chrono::high_resolution_clock::now();
	}
	else
	{
		throw new std::exception("Start is already called");
	}
}

void Time::TimeMeasure::StopMeasure()
{
	if (this->start_was_called)
	{
		this->end_time = std::chrono::high_resolution_clock::now();
		this->start_was_called = false;
	}
	else
	{
		throw new std::exception("Start was not called");
	}
}

std::chrono::seconds Time::TimeMeasure::GetDifference() const
{
	return std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
}