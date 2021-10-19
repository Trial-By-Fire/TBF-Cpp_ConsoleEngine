#pragma once



// Includes

#include "OSPlatform.hpp"
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

	static const Data& GetContext(void);

	static void TakeInitialSnapshot(void);

	static void TakeEndingSnapshot(void);

	static void Update(void);

private:

	// Static Data

	static Data Context;
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
