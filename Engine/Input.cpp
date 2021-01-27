// Parent Header
#include "Input.hpp"



// Includes
#include "Cycler.hpp"
#include "Memory.hpp"



// Static Data

// Private

InputData Input;



// Forward Declarations

EKeyCode GetKeyCodeAtIndex  (size_t _index);
size_t GetKeyIndexFromCode(EKeyCode _key);



// Functions

// Public

void Input_LoadModule(void)
{
	//Input.StateIndex = 0;
}

const InputData* Input_GetContext(void)
{
	return &Input;
}

void Input_Update(void)
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

		EInputState* CurrentState = &Input.KeyStates[index];
				
		EInputState latestState;

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

void Input_SubscribeTo(EKeyCode _key, InputEvent_Function* _callbackFunction)
{
	Subscriptions* subs = &Input.KeyEventSubs[GetKeyIndexFromCode(_key)];

	if (subs->Num == 0)
	{
		subs->Array = (InputEvent_FunctionPtr*)GlobalAllocate(InputEvent_FunctionPtr*, 1);

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


		Address resizeIntermediary = GlobalReallocate( InputEvent_FunctionPtr*, subs->Array, (subs->Num + 1) );

		if (resizeIntermediary != NULL)
		{
			subs->Array = (InputEvent_FunctionPtr*)resizeIntermediary;

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

void Input_Unsubscribe(EKeyCode _key, InputEvent_Function* _callbackFunction)
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
			return Key_Arrow_Up;
		}
		case 1:
		{
			return Key_Arrow_Down;
		}
		case 2:
		{
			return Key_Arrow_Left;
		}
		case 3:
		{
			return Key_Arrow_Right;
		}
		case 4:
		{
			return Key_Enter;
		}
		case 5:
		{
			return Key_Tab;
		}
	}
}

size_t GetKeyIndexFromCode(EKeyCode _key)
{
	switch (_key)
	{
		case Key_Arrow_Up:
		{
			return 0;
		}
		case Key_Arrow_Down:
		{
			return 1;
		}
		case Key_Arrow_Left:
		{
			return 2;
		}
		case Key_Arrow_Right:
		{
			return 3;
		}
		case Key_Enter:
		{
			return 4;
		}
		case Key_Tab:
		{
			return 5;
		}
	}
}
