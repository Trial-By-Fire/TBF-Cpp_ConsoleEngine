#pragma once



// Includes

#include "OSPlatform.hpp"
#include "FloatCompare.hpp"
#include "LAL.hpp"



class Timing
{
	friend class Timer;

public:

	struct Data
	{
		LARGE_INTEGER TimeSnapshot_Initial {};
		LARGE_INTEGER TimeSnapshot_End     {};
		LARGE_INTEGER TimeFrequency        {};
		sInt64        Cycle_TicksElapsed   = 0;
		float64       Cycle_Microseconds   = 0.0L;
		float64       DeltaTime            = 0.0L;
	};



	// Constants

	unbound CompileTime sInt64 TickToMicroseconds  = 1000000;
	unbound CompileTime sInt64 MicrosecondToSecond = 10000000;



	// Functions

	unbound ro Data& GetContext(void);

	unbound void LoadModule(void);

	unbound void TakeInitialSnapshot(void);

	unbound void TakeEndingSnapshot(void);

	unbound void Update(void);

private:

	unbound void InitalizeData(void);


	// Static Data

	unbound Data Context;
};

	

class Timer
{
public:

	bool Ended();
	void Reset();
	void Tick ();


	float64 Elapsed = 0;
	float64 EndTime = 0;
};
