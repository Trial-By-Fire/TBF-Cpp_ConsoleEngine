#pragma once



// Includes

#include "C_STL.hpp"
#include "OSPlatform.hpp"
#include "LAL.hpp"




namespace Input
{
	using OSPlatform::EKeyCode;


	// Constants

	CompileTime uIntDM Keys_NumTracked = 6;



	// Enums

	enum class EState
	{
		None,
		Released,
		Pressed,
		PressHeld
	};



	// Aliases (Typedefs)

	using BoolArray_KeySignals = bool[Keys_NumTracked];

	using EventFunction = void(EState);

	using EventFunctionPtr = EventFunction*;



	// Structures

	struct StructOf_KeySignals
	{
		bool Up;
		bool Down;
		bool Left;
		bool Right;
		bool Enter;
		bool Tab;
	};

	struct SignalState
	{
		union
		{
			BoolArray_KeySignals Array;
			StructOf_KeySignals  Keys;
		};
	};

	struct Subscriptions
	{
		EventFunctionPtr* Array;

		size_t Num;
	};

	struct Data
	{
		// Signal Buffer

		SignalState CurrentSignalState ;
		SignalState PreviousSignalState;

		EState KeyStates[Keys_NumTracked];

		// Key Event Subscriptions

		Subscriptions KeyEventSubs[Keys_NumTracked];
	};



	// Functions

	ro Data* GetContext(void);

	void LoadModule(void);

	void Update(void);

	void SubscribeTo(EKeyCode _key, EventFunction* _callbackFunction);

	void Unsubscribe(EKeyCode _key, EventFunction* _callbackFunction);
}

