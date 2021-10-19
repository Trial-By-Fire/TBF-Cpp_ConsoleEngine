// Parent Header
#include "Input.hpp"



// Includes
#include "Cycler.hpp"
#include "Memory.hpp"




// Usings

using OSPlatform::EKeyCode;



// Static Data

// Private

Input::Data Input::Context;



// Forward Declarations

EKeyCode GetKeyCodeAtIndex  (uIntDM   _index);
uIntDM   GetKeyIndexFromCode(EKeyCode _key);



// Functions

// Public

const Input::Data& Input::GetContext(void)
{
	return Context;
}

void Input::Update(void)
{
	for (uIntDM index = 0; index < Keys_NumTracked; index++)
	{
		bool Current, Previous;

		// Get current signal state

		Context.PreviousSignalState.Array[index] = Context.CurrentSignalState.Array[index];

		Previous = Context.CurrentSignalState.Array[index];

		Context.CurrentSignalState.Array[index] = GetKeySignal(GetKeyCodeAtIndex(index));

		Current = Context.CurrentSignalState.Array[index];

		// Determine latest key state.

		EState& CurrentState = Context.KeyStates[index];
				
		EState latestState = EState::None;

		if (Current == Previous)
		{
			if (Current == true)
			{
				latestState = EState::PressHeld;
			}
			else
			{
				if (CurrentState != EState::PressHeld)
				{
					latestState = EState::None;
				}
			}
		}
		else
		{
			if (Current == false)
			{
				latestState = EState::Released;
			}
			else
			{
				latestState = EState::Pressed;
			}
		}

		if (latestState != CurrentState)
		{
			CurrentState = latestState;

			for (auto& sub : Context.KeyEventSubs[index])
			{
				if (sub != nullptr)
				{
					dref(sub)(CurrentState);
				}
			}
		}
	}
}

void Input::SubscribeTo(EKeyCode _key, EventFunction& _callbackFunction)
{
	Subscriptions& subs = Context.KeyEventSubs[GetKeyIndexFromCode(_key)];

	for (auto& sub : subs)
	{
		if (sub == nullptr)
		{
			sub = getPtr(_callbackFunction);

			return;
		}
	}

	subs.push_back(getPtr(_callbackFunction));
}

void Input::Unsubscribe(EKeyCode _key, EventFunction& _callbackFunction)
{
	Subscriptions& subs = Context.KeyEventSubs[GetKeyIndexFromCode(_key)];

	for (auto& sub : subs)
	{
		if (sub == _callbackFunction)
		{
			sub = nullptr;
		}
	}
}



// Private

EKeyCode GetKeyCodeAtIndex(uIntDM _index)
{
	switch (_index)
	{
		case 0:
		{
			return EKeyCode::Arrow_Up;
		}
		case 1:
		{
			return EKeyCode::Arrow_Down;
		}
		case 2:
		{
			return EKeyCode::Arrow_Left;
		}
		case 3:
		{
			return EKeyCode::Arrow_Right;
		}
		case 4:
		{
			return EKeyCode::Enter;
		}
		case 5:
		{
			return EKeyCode::Tab;
		}
	}

	return EKeyCode::None;
}

uIntDM GetKeyIndexFromCode(EKeyCode _key)
{
	switch (_key)
	{
		case EKeyCode::Arrow_Up:
		{
			return 0;
		}
		case EKeyCode::Arrow_Down:
		{
			return 1;
		}
		case EKeyCode::Arrow_Left:
		{
			return 2;
		}
		case EKeyCode::Arrow_Right:
		{
			return 3;
		}
		case EKeyCode::Enter:
		{
			return 4;
		}
		case EKeyCode::Tab:
		{
			return 5;
		}
	}

	return NumLimits<uIntDM>::max();
}

