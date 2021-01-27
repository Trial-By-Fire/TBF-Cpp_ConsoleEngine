#pragma once



// Includes

#include "C_STL.hpp"
#include "OSPlatform.hpp"
#include "LAL.hpp"




namespace Input
{
	using OSPlatform::EKeyCode;



	// Enums

	enum Constants
	{
		Keys_NumTracked = 6,

		InputState_BufferSize = 10
	};

	enum EState
	{
		EInput_None,
		EInput_Released,
		EInput_Pressed,
		EInput_PressHeld
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



	// Constants

	#define SizeOf_InputSystem \
		sizeof(Data)



	// Functions

	const Data* GetContext(void);

	void LoadModule(void);

	void Update(void);

	void SubscribeTo(EKeyCode _key, EventFunction* _callbackFunction);

	void Unsubscribe(EKeyCode _key, EventFunction* _callbackFunction);
}

