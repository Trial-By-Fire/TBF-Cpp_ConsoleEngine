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
		SteadyTimePoint TimeSnapshot_Initial;
		SteadyTimePoint TimeSnapshot_End    ;
		Duration64      DeltaTime           ;
	};


	// Functions

	unbound ro Data& GetContext(void);

	unbound void TakeInitialSnapshot(void);

	unbound void TakeEndingSnapshot(void);

	unbound void Update(void);

private:

	// Static Data

	unbound Data Context;
};

	

class Timer
{
public:

	Timer(float64 _endTime);

	bool Ended();
	void Reset();
	void Tick ();


	Duration64 Elapsed;
	Duration64 EndTime;
};
