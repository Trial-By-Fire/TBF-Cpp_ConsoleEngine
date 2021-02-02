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

void Input::LoadModule(void)
{
}

ro Input::Data& Input::GetContext(void)
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

		ptr<EState> CurrentState = getPtr(Context.KeyStates[index]);
				
		EState latestState = EState::None;

		if (Current == Previous)
		{
			if (Current == true)
			{
				latestState = EState::PressHeld;
			}
			else
			{
				if (dref(CurrentState) != EState::PressHeld)
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

		if (latestState != *CurrentState)
		{
			dref(CurrentState) = latestState;

			uIntDM num = Context.KeyEventSubs[index].Num;

			for (uIntDM subIndex = 0; subIndex < num; subIndex++)
			{
				if ( Context.KeyEventSubs[index].Array[subIndex] != nullptr)
				{
					Context.KeyEventSubs[index].Array[subIndex](*CurrentState);
				}
			}
		}
	}
}

void Input::SubscribeTo(EKeyCode _key, EventFunction& _callbackFunction)
{
	ptr<Subscriptions> subs = getPtr(Context.KeyEventSubs[GetKeyIndexFromCode(_key)] );

	if (subs->Num == 0)
	{
		subs->Array = Memory::GlobalAllocate<EventFunctionPtr>(1);

		subs->Num++;
	}
	else
	{
		for (uIntDM subIndex = 0; subIndex < subs->Num; subIndex++)
		{
			if ( getPtr(subs->Array)[subIndex] == nullptr)
			{
				subs->Array[subs->Num - 1] = getPtr(_callbackFunction);

				return;
			}
		}


		ptr<EventFunctionPtr> resizeIntermediary = Memory::GlobalReallocate(subs->Array, (subs->Num + 1));

		if (resizeIntermediary != nullptr)
		{
			subs->Array = resizeIntermediary;

			subs->Num++;
		}
		else
		{
			perror("Failed to globally reallocate subscription array.");

			exit(1);
		}
	}

	subs->Array[subs->Num - 1] = &_callbackFunction;
}

void Input::Unsubscribe(EKeyCode _key, EventFunction& _callbackFunction)
{
	ptr<Subscriptions> subs = getPtr(Context.KeyEventSubs[GetKeyIndexFromCode(_key)] );

	for (uIntDM subIndex = 0; subIndex < subs->Num; subIndex++)
	{
		if (subs->Array[subIndex] == &_callbackFunction)
		{
			subs->Array[subIndex] = nullptr;
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

	return ULONG_MAX;
}

