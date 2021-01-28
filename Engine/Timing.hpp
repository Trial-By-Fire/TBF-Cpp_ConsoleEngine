#pragma once



// Includes

#include "OSPlatform.hpp"
#include "FloatCompare.hpp"
#include "LAL.hpp"



namespace Timing
{
	// Structures

	struct Data
	{
		LARGE_INTEGER TimeSnapshot_Initial;
		LARGE_INTEGER TimeSnapshot_End    ;
		LARGE_INTEGER TimeFrequency       ;
		sInt64        Cycle_TicksElapsed  ;
		float64       Cycle_Microseconds  ;
		float64       DeltaTime           ;
	};



	// Constants

	CompileTime sInt64 TickToMicroseconds  = 1000000;
	CompileTime sInt64 MicrosecondToSecond = 10000000;



	// Functions

	ro Data* GetContext(void);

	void LoadModule(void);

	void TakeInitialSnapshot(void);

	void TakeEndingSnapshot(void);

	void Update(void);




	// Structs

	struct TimerData
	{
		float64 Elapsed;
		float64 EndTime;
	};



	// Functions

	bool Timer_Ended(TimerData* _timer);

	void Timer_Reset(TimerData* _timer);

	void Timer_Tick(TimerData* _timer);
}
