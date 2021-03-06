#pragma once



// Includes

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

	using EventFunctionPtr = ptr<EventFunction>;
	
	using Subscriptions = DynamicArray<EventFunctionPtr>;


	// Constants

	unbound CompileTime uIntDM Keys_NumTracked = 6;;


	// Structures

	struct SignalState
	{
		SignalState() {};

		struct StructOf_KeySignals
		{
			bool Up    = false;
			bool Down  = false;
			bool Left  = false;
			bool Right = false;
			bool Enter = false;
			bool Tab   = false;
		};

		using BoolArray_KeySignals = StaticArray<bool, Keys_NumTracked>;


		union
		{
			BoolArray_KeySignals Array;
			StructOf_KeySignals  Keys;
		};
	};

	struct Data
	{
		// Signal Buffer

		SignalState CurrentSignalState ;
		SignalState PreviousSignalState;

		StaticArray<EState, Keys_NumTracked> KeyStates;

		// Key Event Subscriptions

		StaticArray<Subscriptions, Keys_NumTracked> KeyEventSubs;
	};



	// Functions

	unbound ro Data& GetContext(void);

	unbound void Update(void);

	unbound void SubscribeTo(EKeyCode _key, EventFunction& _callbackFunction);

	unbound void Unsubscribe(EKeyCode _key, EventFunction& _callbackFunction);


private:

	unbound Data Context;
};

