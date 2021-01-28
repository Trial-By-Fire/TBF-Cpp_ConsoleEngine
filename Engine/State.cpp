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

State CurrentState;

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

	CurrentState.data = nullptr;

	CurrentState.Set(Intro::GetState());

	Input::SubscribeTo(EKeyCode::Arrow_Up  , &State_OnKeyArrowUp  );
	Input::SubscribeTo(EKeyCode::Arrow_Down, &State_OnKeyArrowDown);
	Input::SubscribeTo(EKeyCode::Enter     , &State_OnKeyTab      );
}

ro State* State::GetEngineState()
{
	return &CurrentState;
}

void State::SetEngineState(State::Callbacks* _state)
{
	if (CurrentState.data != nullptr)
	{
		CurrentState.data->Unload();
	}

	CurrentState.data = _state;

	CurrentState.data->Load();
}

void State::Set(Callbacks* _state)
{
	if (data != nullptr)
	{
		data->Unload();
	}

	data = _state;

	data->Load();
}

void State::Unload(void) ro
{
	data->Unload();
}

void State::Update(void) ro
{
	if (data != nullptr)
	{
		data->Update();
	}
}

void State::Render(void) ro
{
	if (data != nullptr)
	{
		data->Render();
	}
}

void State::LoadGame(void)
{
	CurrentState.Set(Engine::LoadGame());
}

