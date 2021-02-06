// Parent Header
#include "Timing.hpp"



// Includes
#include "Memory.hpp"
#include "Renderer.hpp"



// Static Data

// Private

Timing::Data Timing::Context;



// Functions

// Public

ro Timing::Data& Timing::GetContext(void)
{
	return Context;
}

void Timing::TakeInitialSnapshot(void)
{
	Context.TimeSnapshot_Initial = SteadyClock::now();
}

void Timing::TakeEndingSnapshot(void)
{
	Context.TimeSnapshot_End = SteadyClock::now();
}

void Timing::Update(void)
{
	Context.DeltaTime = std::chrono::duration_cast<Duration64>(Context.TimeSnapshot_End - Context.TimeSnapshot_Initial);

	Renderer::ProcessTiming();
}



// Timer

Timer::Timer(float64 _endTime) :
	EndTime(_endTime)
{}

bool Timer::Ended()
{
	return Elapsed >= EndTime;
}

void Timer::Reset()
{
	Elapsed = Duration64(0.0);
}

void Timer::Tick()
{
	Elapsed += Timing::Context.DeltaTime;
}

