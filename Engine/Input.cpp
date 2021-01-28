// Parent Header
#include "Input.hpp"



// Includes
#include "Cycler.hpp"
#include "Memory.hpp"



namespace Input
{
	// Static Data

	// Private

	Data Input;



	// Forward Declarations

	EKeyCode GetKeyCodeAtIndex  (uIntDM   _index);
	uIntDM   GetKeyIndexFromCode(EKeyCode _key);



	// Functions

	// Public

	void LoadModule(void)
	{
		//Input.StateIndex = 0;
	}

	ro Data* GetContext(void)
	{
		return &Input;
	}

	void Update(void)
	{
		for (uIntDM index = 0; index < Keys_NumTracked; index++)
		{
			bool Current, Previous;

			// Get current signal state

			Input.PreviousSignalState.Array[index] = Input.CurrentSignalState.Array[index];

			Previous = Input.CurrentSignalState.Array[index];

			Input.CurrentSignalState.Array[index] = GetKeySignal(GetKeyCodeAtIndex(index));

			Current = Input.CurrentSignalState.Array[index];

			// Determine latest key state.

			EState* CurrentState = &Input.KeyStates[index];
					
			EState latestState = EState::None;

			if (Current == Previous)
			{
				if (Current == true)
				{
					latestState = EState::PressHeld;
				}
				else
				{
					if (*CurrentState != EState::PressHeld)
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
				*CurrentState = latestState;

				for (uIntDM subIndex = 0; subIndex < Input.KeyEventSubs[index].Num; subIndex++)
				{
					if ( Input.KeyEventSubs[index].Array[subIndex] != nullptr)
					{
						Input.KeyEventSubs[index].Array[subIndex](*CurrentState);
					}
				}
			}
		}
	}

	void SubscribeTo(EKeyCode _key, EventFunction* _callbackFunction)
	{
		Subscriptions* subs = &Input.KeyEventSubs[GetKeyIndexFromCode(_key)];

		if (subs->Num == 0)
		{
			subs->Array = (EventFunctionPtr*)GlobalAllocate(EventFunctionPtr*, 1);

			subs->Num++;
		}
		else
		{
			for (uIntDM subIndex = 0; subIndex < subs->Num; subIndex++)
			{
				if ( (&subs->Array)[subIndex] == nullptr)
				{
					subs->Array[subs->Num - 1] = _callbackFunction;

					return;
				}
			}


			Memory::Address resizeIntermediary = GlobalReallocate(EventFunctionPtr*, subs->Array, (subs->Num + 1) );

			if (resizeIntermediary != nullptr)
			{
				subs->Array = (EventFunctionPtr*)resizeIntermediary;

				subs->Num++;
			}
			else
			{
				perror("Failed to globally reallocate subscription array.");

				exit(1);
			}
		}

		subs->Array[subs->Num - 1] = _callbackFunction;
	}

	void Unsubscribe(EKeyCode _key, EventFunction* _callbackFunction)
	{
		Subscriptions* subs = &Input.KeyEventSubs[GetKeyIndexFromCode(_key)];

		for (uIntDM subIndex = 0; subIndex < subs->Num; subIndex++)
		{
			if (subs->Array[subIndex] == _callbackFunction)
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
	}
}

