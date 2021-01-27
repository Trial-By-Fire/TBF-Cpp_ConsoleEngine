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

	EKeyCode GetKeyCodeAtIndex  (size_t   _index);
	size_t   GetKeyIndexFromCode(EKeyCode _key);



	// Functions

	// Public

	void LoadModule(void)
	{
		//Input.StateIndex = 0;
	}

	const Data* GetContext(void)
	{
		return &Input;
	}

	void Update(void)
	{
		size_t index = 0; 

		for (; index < Keys_NumTracked; index++)
		{
			bool Current, Previous;

			// Get current signal state

			Input.PreviousSignalState.Array[index] = Input.CurrentSignalState.Array[index];

			Previous = Input.CurrentSignalState.Array[index];

			Input.CurrentSignalState.Array[index] = GetKeySignal(GetKeyCodeAtIndex(index));

			Current = Input.CurrentSignalState.Array[index];

			// Determine latest key state.

			EState* CurrentState = &Input.KeyStates[index];
					
			EState latestState;

			if (Current == Previous)
			{
				if (Current == true)
				{
					latestState = EInput_PressHeld;
				}
				else
				{
					if (*CurrentState != EInput_PressHeld)
					{
						latestState = EInput_None;
					}
				}
			}
			else
			{
				if (Current == false)
				{
					latestState = EInput_Released;
				}
				else
				{
					latestState = EInput_Pressed;
				}
			}

			if (latestState != *CurrentState)
			{
				*CurrentState = latestState;

				for (size_t subIndex = 0; subIndex < Input.KeyEventSubs[index].Num; subIndex++)
				{
					if ( Input.KeyEventSubs[index].Array[subIndex] != NULL)
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
			for (size_t subIndex = 0; subIndex < subs->Num; subIndex++)
			{
				if ( (&subs->Array)[subIndex] == NULL)
				{
					subs->Array[subs->Num - 1] = _callbackFunction;

					return;
				}
			}


			Memory::Address resizeIntermediary = GlobalReallocate(EventFunctionPtr*, subs->Array, (subs->Num + 1) );

			if (resizeIntermediary != NULL)
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

		for (size_t subIndex = 0; subIndex < subs->Num; subIndex++)
		{
			if (subs->Array[subIndex] == _callbackFunction)
			{
				subs->Array[subIndex] = NULL;
			}
		}
	}



	// Private

	EKeyCode GetKeyCodeAtIndex(size_t _index)
	{
		switch (_index)
		{
			case 0:
			{
				return EKeyCode::Key_Arrow_Up;
			}
			case 1:
			{
				return EKeyCode::Key_Arrow_Down;
			}
			case 2:
			{
				return EKeyCode::Key_Arrow_Left;
			}
			case 3:
			{
				return EKeyCode::Key_Arrow_Right;
			}
			case 4:
			{
				return EKeyCode::Key_Enter;
			}
			case 5:
			{
				return EKeyCode::Key_Tab;
			}
		}
	}

	size_t GetKeyIndexFromCode(EKeyCode _key)
	{
		switch (_key)
		{
			case EKeyCode::Key_Arrow_Up:
			{
				return 0;
			}
			case EKeyCode::Key_Arrow_Down:
			{
				return 1;
			}
			case EKeyCode::Key_Arrow_Left:
			{
				return 2;
			}
			case EKeyCode::Key_Arrow_Right:
			{
				return 3;
			}
			case EKeyCode::Key_Enter:
			{
				return 4;
			}
			case EKeyCode::Key_Tab:
			{
				return 5;
			}
		}
	}
}

