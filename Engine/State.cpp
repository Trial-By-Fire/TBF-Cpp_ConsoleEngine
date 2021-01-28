// Parent Header
#include "State.hpp"



// Includes

#include "State_Intro.hpp"
#include "Engine.hpp"



CompileTime auto Logs_ScrollUp   = Renderer::Logs_ScrollUp;
CompileTime auto Logs_ScrollDown = Renderer::Logs_ScrollDown;



enum class EFocusState
{
	Game,
	Logs
};




// State

State EngineState;

EFocusState FocusState = EFocusState::Game;



// Functions

// Public

void State_OnKeyArrowUp(Input::EState _state)
{
	switch (_state)
	{
		case Input::EState::Pressed:
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
		case Input::EState::Pressed:
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
		case Input::EState::Pressed:
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


void State::LoadModule(void)
{
	using OSPlatform::EKeyCode;

	EngineState.Set(Intro::GetState());

	Input::SubscribeTo(EKeyCode::Arrow_Up  , State_OnKeyArrowUp  );
	Input::SubscribeTo(EKeyCode::Arrow_Down, State_OnKeyArrowDown);
	Input::SubscribeTo(EKeyCode::Enter     , State_OnKeyTab      );
}

AState* AState::GetEngineState()
{
	return &EngineState;
}

void AState::SetEngineState(AState* _state)
{
	EngineState.Set(_state);
}

void State::Set(AState* _state)
{
	if (CurrentState != nullptr)
	{
		CurrentState->Unload();
	}

	CurrentState = _state;

	CurrentState->Load();
}

void State::Load(void)
{
	CurrentState->Load();
}

void State::Unload(void)
{
	CurrentState->Unload();
}

void State::Update(void)
{
	if (CurrentState != nullptr)
	{
		CurrentState->Update();
	}
}

void State::Render(void)
{
	if (CurrentState != nullptr)
	{
		CurrentState->Render();
	}
}

