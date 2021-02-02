// Parent Header
#include "Game_LevelState_Paused.hpp"



// Includes

#include "Game_LevelState_Ingame.hpp"
#include "Game_Util.hpp"



namespace Game
{
	class Paused : public AState
	{
	public:

		unbound void PressResume    (void);
		unbound void PressQuitToMenu(void);

		override void Load  (void);
		override void Unload(void);
		override void Update(void);
		override void Render(void);
	};



	// Static Data

	// Private

	Paused Paused_State;

	UI_Widget PauseWidget;

	bool Paused_DoneOnce = false;

	uInt PauseUI_Index = 0;



	// Enums

	enum
	{
		EUI_Resume     = 0,
		EUI_QuitToMenu = 1
	};



	// Functions

	// Class Public

	void Paused::PressResume(void)
	{
		LevelState::Set(GetIngameState());
	}

	void Paused::PressQuitToMenu(void)
	{
		Ingame_Reload();

		State::SetEngineState(Engine::LoadGame());
	}

	void LevelState_Paused_OnKeyEnter(Input::EState _state)
	{
		switch (_state)
		{
			case Input::EState::Pressed:
			{
				PauseWidget.Select();

				break;
			}
		}
	}

	void LevelState_Paused_OnKeyArrowUp(Input::EState  _state)
	{
		switch (_state)
		{
			case Input::EState::Pressed:
			{
				WriteToLog(L"Pause: On Key Up");

				PauseWidget.MoveUp();

				break;
			}
		}
	}

	void LevelState_Paused_OnKeyArrowDown(Input::EState  _state)
	{
		switch (_state)
		{
			case Input::EState::Pressed:
			{
				WriteToLog(L"Pause: On Key Down");

				PauseWidget.MoveDown();

				break;
			}
		}
	}

	void Paused::Load(void)
	{
		PauseUI_Index = 0;

		if (! Paused_DoneOnce)
		{
			PauseWidget.TextUIs           = nullptr;
			PauseWidget.Num_TextUIs       = 0;
			PauseWidget.Grid.Buttons      = nullptr;
			PauseWidget.Grid.Num          = 0;
			PauseWidget.Grid.CurrentIndex = 0;


			COORD startCell, endCell;

			startCell.X = 0; endCell.X = 0;
			startCell.Y = 9; endCell.Y = 9;

			PauseWidget.AddText
			(
				L"Paused\0",
				startCell, 
				endCell,
				true   // Should Center
			);

			startCell.Y = 15; endCell.Y = 15;

			PauseWidget.AddButton
			(
				L"Resume\0",
				startCell, endCell,
				true,
				Paused::PressResume
			);

			startCell.X = 0;  endCell.X = 0;
			startCell.Y = 17; endCell.Y = 17;

			PauseWidget.AddButton
			(
				L"Quit to Menu\0",
				startCell, endCell,
				true,
				Paused::PressQuitToMenu
			);

			Paused_DoneOnce = true;
		}

		Input::SubscribeTo(EKeyCode::Enter     , LevelState_Paused_OnKeyEnter    );
		Input::SubscribeTo(EKeyCode::Arrow_Up  , LevelState_Paused_OnKeyArrowUp  );
		Input::SubscribeTo(EKeyCode::Arrow_Down, LevelState_Paused_OnKeyArrowDown);
	}

	void Paused::Unload(void)
	{
		Input::Unsubscribe(EKeyCode::Enter     , LevelState_Paused_OnKeyEnter    );
		Input::Unsubscribe(EKeyCode::Arrow_Up  , LevelState_Paused_OnKeyArrowUp  );
		Input::Unsubscribe(EKeyCode::Arrow_Down, LevelState_Paused_OnKeyArrowDown);
	}

	void Paused::Update(void)
	{
	}

	void Paused::Render(void)
	{
		PauseWidget.Render();
	}



	// Public

	ptr<AState> GetPausedState(void)
	{
		return getPtr(Paused_State);
	}
}

