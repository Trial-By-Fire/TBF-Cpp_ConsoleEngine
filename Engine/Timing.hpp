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
		LARGE_INTEGER TimeSnapshot_Initial;
		LARGE_INTEGER TimeSnapshot_End;
		LARGE_INTEGER TimeFrequency;
		sInt64        Cycle_TicksElapsed;
		float64       Cycle_Microseconds;
		float64       DeltaTime;
	};



	// Constants

	unbound CompileTime sInt64 TickToMicroseconds  = 1000000;
	unbound CompileTime sInt64 MicrosecondToSecond = 10000000;



	// Functions

	unbound ro Data* GetContext(void);

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


	float64 Elapsed;
	float64 EndTime;
};
