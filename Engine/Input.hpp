#pragma once



// Includes

#include "C_STL.hpp"
#include "OSPlatform.hpp"
#include "LAL.hpp"



class Input
{
public:

	using EKeyCode = OSPlatform::EKeyCode;


	// Enums

	enum class EState
	{
		None,
		Released,
		Pressed,
		PressHeld
	};



	// Aliases (Typedefs)

	using EventFunction = void(EState);

	using EventFunctionPtr = EventFunction*;


	// Constants

	unbound CompileTime uIntDM Keys_NumTracked = 6;;


	// Structures

	struct SignalState
	{
		struct StructOf_KeySignals
		{
			bool Up;
			bool Down;
			bool Left;
			bool Right;
			bool Enter;
			bool Tab;
		};

		using BoolArray_KeySignals = bool[Keys_NumTracked];


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

	unbound ro Data* GetContext(void);

	unbound void LoadModule(void);

	unbound void Update(void);

	unbound void SubscribeTo(EKeyCode _key, EventFunction* _callbackFunction);

	unbound void Unsubscribe(EKeyCode _key, EventFunction* _callbackFunction);


private:

	unbound Data Context;
};

