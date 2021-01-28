// Parent Header
#include "State.hpp"



// Includes

#include "State_Intro.hpp"
#include "Engine.hpp"



namespace State
{
	using Input::EState;

	using Renderer::Logs_ScrollUp;
	using Renderer::Logs_ScrollDown;



	enum class EFocusState
	{
		Game,
		Logs
	};




	// State

	StateObj* CurrentState = nullptr;

	EFocusState FocusState = EFocusState::Game;



	// Functions

	// Public

	void State_OnKeyArrowUp(Input::EState _state)
	{
		switch (_state)
		{
			case EState::Pressed:
			{
				switch (FocusState)
				{
					case EFocusState::Logs:
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
			case EState::Pressed:
			{
				switch (FocusState)
				{
					case EFocusState::Logs:
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
			case EState::Pressed:
			{
				switch (FocusState)
				{
					case EFocusState::Game:
					{
						FocusState = EFocusState::Logs;

						break;
					}
					case EFocusState::Logs:
					{
						FocusState = EFocusState::Game;

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

		Input::SubscribeTo(EKeyCode::Arrow_Up  , &State_OnKeyArrowUp  );
		Input::SubscribeTo(EKeyCode::Arrow_Down, &State_OnKeyArrowDown);
		Input::SubscribeTo(EKeyCode::Enter     , &State_OnKeyTab      );
	}

	void SetState(StateObj* _state)
	{
		if (CurrentState != nullptr)
		{
			CurrentState->Unload();
		}

		CurrentState = _state;

		CurrentState->Load();
	}

	void Update(void)
	{
		if (CurrentState != nullptr)
		{
			CurrentState->Update();
		}
	}

	void Render(void)
	{
		if (CurrentState != nullptr)
		{
			CurrentState->Render();
		}
	}

	void LoadGame(void)
	{
		SetState(Engine::LoadGame());
	}
}

