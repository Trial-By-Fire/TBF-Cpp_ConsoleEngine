// Parent Header
#include "Game_LevelState_Paused.hpp"



// Includes

#include "Game_LevelState_Ingame.hpp"
#include "Game_Util.hpp"



namespace Game
{
	// Static Data

	// Private

	StateObj Paused_State;

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

	void Level_State_Paused_PressResume(void)
	{
		LevelState_SetSubstate(GetIngameState());
	}

	void Level_State_Paused_PressQuitToMenu(void)
	{
		Ingame_Reload();

		State::SetState(Engine::LoadGame());
	}

	void LevelState_Paused_OnKeyEnter(Input::EState _state)
	{
		switch (_state)
		{
			case Input::EInput_Pressed:
			{
				UI_Widget_Select(&PauseWidget);

				break;
			}
		}
	}

	void LevelState_Paused_OnKeyArrowUp(Input::EState  _state)
	{
		switch (_state)
		{
			case Input::EInput_Pressed:
			{
				WriteToLog((WideChar*)L"Pause: On Key Up");

				UI_Widget_MoveUp(&PauseWidget);

				break;
			}
		}
	}

	void LevelState_Paused_OnKeyArrowDown(Input::EState  _state)
	{
		switch (_state)
		{
			case Input::EInput_Pressed:
			{
				WriteToLog((WideChar*)L"Pause: On Key Down");

				UI_Widget_MoveDown(&PauseWidget);

				break;
			}
		}
	}

	void Load_Paused(void)
	{
		PauseUI_Index = 0;

		if (! Paused_DoneOnce)
		{
			PauseWidget.TextUIs           = NULL;
			PauseWidget.Num_TextUIs       = 0;
			PauseWidget.Grid.Buttons      = NULL;
			PauseWidget.Grid.Num          = 0;
			PauseWidget.Grid.CurrentIndex = 0;


			COORD startCell, endCell;

			startCell.X = 0; endCell.X = 0;
			startCell.Y = 9; endCell.Y = 9;

			UI_Widget_AddText
			(
				&PauseWidget,

				L"Paused\0",
				startCell, 
				endCell,
				true   // Should Center
			);

			startCell.Y = 15; endCell.Y = 15;

			UI_Widget_AddButton
			(
				&PauseWidget,

				L"Resume\0",
				startCell, endCell,
				true,
				&Level_State_Paused_PressResume
			);

			startCell.X = 0;  endCell.X = 0;
			startCell.Y = 17; endCell.Y = 17;

			UI_Widget_AddButton
			(
				&PauseWidget,

				L"Quit to Menu\0",
				startCell, endCell,
				true,
				&Level_State_Paused_PressQuitToMenu
			);

			Paused_DoneOnce = true;
		}

		Input::SubscribeTo(EKeyCode::Key_Enter     , &LevelState_Paused_OnKeyEnter    );
		Input::SubscribeTo(EKeyCode::Key_Arrow_Up  , &LevelState_Paused_OnKeyArrowUp  );
		Input::SubscribeTo(EKeyCode::Key_Arrow_Down, &LevelState_Paused_OnKeyArrowDown);
	}

	void Unload_Paused(void)
	{
		Input::Unsubscribe(EKeyCode::Key_Enter     , &LevelState_Paused_OnKeyEnter    );
		Input::Unsubscribe(EKeyCode::Key_Arrow_Up  , &LevelState_Paused_OnKeyArrowUp  );
		Input::Unsubscribe(EKeyCode::Key_Arrow_Down, &LevelState_Paused_OnKeyArrowDown);
	}

	void Update_Paused(void)
	{
	}

	void Render_Paused(void)
	{
		UI_Widget_Render(&PauseWidget);
	}



	// Public

	StateObj* GetPausedState(void)
	{
		static bool stateConstructed = false;

		if (! stateConstructed)
		{
			Paused_State.Load   = &Load_Paused  ;
			Paused_State.Unload = &Unload_Paused;
			Paused_State.Update = &Update_Paused;
			Paused_State.Render = &Render_Paused;

			stateConstructed = true;
		}

		return &Paused_State;
	}
}

