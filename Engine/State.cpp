// Parent Header
#include "State.hpp"



// Includes

#include "State_Intro.hpp"
#include "Engine.hpp"



namespace State
{
	using Input::EInput_Pressed;

	using Renderer::Logs_ScrollUp;
	using Renderer::Logs_ScrollDown;



	enum 
	{
		EFocusState_Game,
		EFocusState_Logs
	};




	// State

	StateObj* CurrentState = NULL;

	uInt FocusState = EFocusState_Game;



	// Functions

	// Public

	void State_OnKeyArrowUp(Input::EState _state)
	{
		switch (_state)
		{
			case EInput_Pressed:
			{
				switch (FocusState)
				{
					case EFocusState_Logs:
					{
						Logs_ScrollUp();

						break;
					}
				}

				break;
			}
		}
	}

	void State_OnKeyArrowDown(Input::EState _state)
	{
		switch (_state)
		{
			case EInput_Pressed:
			{
				switch (FocusState)
				{
					case EFocusState_Logs:
					{
						Logs_ScrollDown();

						break;
					}
				}

				break;
			}
		}
	}

	void State_OnKeyTab(Input::EState _state)
	{
		switch (_state)
		{
			case EInput_Pressed:
			{
				switch (FocusState)
				{
					case EFocusState_Game:
					{
						FocusState = EFocusState_Logs;

						break;
					}
					case EFocusState_Logs:
					{
						FocusState = EFocusState_Game;

						break;
					}
				}

				break;
			}
		}
	}


	void LoadModule(void)
	{
		using OSPlatform::EKeyCode;

		CurrentState = Intro::GetState();

		CurrentState->Load();

		Input::SubscribeTo(EKeyCode::Key_Arrow_Up  , &State_OnKeyArrowUp  );
		Input::SubscribeTo(EKeyCode::Key_Arrow_Down, &State_OnKeyArrowDown);
		Input::SubscribeTo(EKeyCode::Key_Enter     , &State_OnKeyTab      );
	}

	void SetState(StateObj* _state)
	{
		if (CurrentState != NULL)
		{
			CurrentState->Unload();
		}

		CurrentState = _state;

		CurrentState->Load();
	}

	void Update(void)
	{
		if (CurrentState != NULL)
		{
			CurrentState->Update();
		}
	}

	void Render(void)
	{
		if (CurrentState != NULL)
		{
			CurrentState->Render();
		}
	}

	void LoadGame(void)
	{
		SetState(Engine::LoadGame());
	}
}

