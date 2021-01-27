// Parent Header
#include "Timing.hpp"



// Includes
#include "Memory.hpp"
#include "Renderer.hpp"



// Static Data

// Private

TimingData Timing;



// Forward Declarations

void Timing_InitalizeData(void);



// Functions

// Public

const TimingData* Timing_GetContext(void)
{
	return &Timing;
}

void Timing_LoadModule(void)
{
	Timing_InitalizeData();
}

void Timing_TakeInitialSnapshot(void)
{
	QueryPerformanceCounter(&Timing.TimeSnapshot_Initial);
}

void Timing_TakeEndingSnapshot(void)
{
	QueryPerformanceCounter(&Timing.TimeSnapshot_End);
}

void Timing_Update(void)
{
	Timing.Cycle_TicksElapsed = Timing.TimeSnapshot_End.QuadPart - Timing.TimeSnapshot_Initial.QuadPart;

	Timing.Cycle_Microseconds = (float64)(Timing.Cycle_TicksElapsed * TickToMicroseconds);
	Timing.Cycle_Microseconds = Timing.Cycle_Microseconds / (float64)Timing.TimeFrequency.QuadPart;

	Timing.DeltaTime = (float64)Timing.Cycle_TicksElapsed / (float64)MicrosecondToSecond;

	Renderer_ProcessTiming(Timing.DeltaTime);
}

// Private

void Timing_InitalizeData(void)
{
	Timing.Cycle_TicksElapsed = 0;
	Timing.Cycle_Microseconds = 0.0L;
	Timing.DeltaTime          = 0.0L;

	QueryPerformanceFrequency(&Timing.TimeFrequency);

	return;
}



// Timer Class

// Functions

// Public

bool Timer_Ended(TimerData* _timer)
{
	return 
		Float64_ApproxGreater(_timer->Elapsed, _timer->EndTime) || 
		Float64_ApproxEqual  (_timer->Elapsed, _timer->EndTime)   ;
}

void Timer_Reset(TimerData* _timer)
{
	_timer->Elapsed = 0.0L;
}

void Timer_Tick(TimerData* _timer)
{
	if (Float64_ApproxEqual(Timing.DeltaTime, 0.000001L) || Float64_ApproxLess(Timing.DeltaTime, 0.000001L))
	{
		_timer->Elapsed = _timer->Elapsed + 0.000001L;

		return;
	}
	else
	{
		_timer->Elapsed = _timer->Elapsed + Timing.DeltaTime;

		return;
	}
}
