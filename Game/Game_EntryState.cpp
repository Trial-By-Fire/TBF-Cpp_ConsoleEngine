// Parent Header
#include "Game_EntryState.hpp"



// Include

#include "Game_LevelState.hpp"
#include "Game_Util.hpp"



namespace Game
{
	// Static Data

	// Private

	StateObj MainMenu;

	UI_Widget MenuWidget;

	bool Menu_DoneOnce = false;

	bool 
		Log_Load   = true,
		Log_Unload = true ;



	// Functions

	// Public Class

	void MainMenu_PressStart(void)
	{
		WriteToLog((WideChar*)L"UI Start Selected");

		State::SetState(GetLevelState());
	}

	void MainMenu_PressQuit(void)
	{
		Cycler::Quit();
	}

	void Game_EntryState_OnKeyArrowUp(Input::EState _state)
	{
		switch (_state)
		{
			case Input::EInput_Pressed:
			{
				WriteToLog((WideChar*)L"EntryState: On Key Up");

				UI_Widget_MoveUp(&MenuWidget);

				break;
			}
		}
	}

	void Game_EntryState_OnKeyArrowDown(Input::EState _state)
	{
		switch (_state)
		{
			case Input::EInput_Pressed:
			{
				WriteToLog((WideChar*)L"EntryState: On Key Down");

				UI_Widget_MoveDown(&MenuWidget);

				break;
			}
		}
	}

	void Game_EntryState_OnKeyEnter(Input::EState _state)
	{
		switch (_state)
		{
			case Input::EInput_Pressed:
			{
				WriteToLog((WideChar*)L"EntryState: On Key Enter");

				UI_Widget_Select(&MenuWidget);

				break;
			}
		}
	}

	void MainMenu_Load(void)
	{
		if (! Menu_DoneOnce)
		{
			MenuWidget.TextUIs           = NULL;
			MenuWidget.Num_TextUIs       = 0;
			MenuWidget.Grid.Buttons      = NULL;
			MenuWidget.Grid.Num          = 0;
			MenuWidget.Grid.CurrentIndex = 0;

			COORD startCell, endCell;

			startCell.X = 0; endCell.X = 0;
			startCell.Y = 9; endCell.Y = 9;


			UI_Widget_AddText
			(
				&MenuWidget,

				L"Generic Platformer Demo\0",
				startCell, 
				endCell,
				true   // Should Center
			);

			startCell.Y = 15; endCell.Y = 15;

			UI_Widget_AddButton
			(
				&MenuWidget,

				L"Start\0",
				startCell, endCell,
				true,
				&MainMenu_PressStart
			);

			startCell.X = -1; endCell.X = -1;
			startCell.Y = 17; endCell.Y = 17;

			UI_Widget_AddButton
			(
				&MenuWidget,

				L"Quit\0",
				startCell, endCell,
				true,
				&MainMenu_PressQuit
			);

			Menu_DoneOnce = true;
		}

		Input::SubscribeTo(EKeyCode::Key_Arrow_Up  , &Game_EntryState_OnKeyArrowUp  );
		Input::SubscribeTo(EKeyCode::Key_Arrow_Down, &Game_EntryState_OnKeyArrowDown);
		Input::SubscribeTo(EKeyCode::Key_Enter     , &Game_EntryState_OnKeyEnter    );

		if (Log_Load)
		{
			WriteToLog((WideChar*)L"Loaded: Main Menu");

			Log_Load = false;
		}
	}

	void MainMenu_Unload(void)
	{
		Input::Unsubscribe(EKeyCode::Key_Arrow_Up  , &Game_EntryState_OnKeyArrowUp  );
		Input::Unsubscribe(EKeyCode::Key_Arrow_Down, &Game_EntryState_OnKeyArrowDown);
		Input::Unsubscribe(EKeyCode::Key_Enter     , &Game_EntryState_OnKeyEnter    );

		if (Log_Unload)
		{
			WriteToLog((WideChar*)L"Unload: Main Menu");

			Log_Unload = false;
		}
	}

	void MainMenu_Update(void)
	{
	}

	void MainMenu_Render(void)
	{
		UI_Widget_Render(&MenuWidget);
	}
}


namespace Engine
{
	// Engine Entrypoint

	StateObj* LoadGame(void)
	{
		using namespace Game;

		static bool stateConstructed = false;

		if (!stateConstructed)
		{
			MainMenu.Load = &MainMenu_Load;
			MainMenu.Unload = &MainMenu_Unload;
			MainMenu.Update = &MainMenu_Update;
			MainMenu.Render = &MainMenu_Render;

			stateConstructed = true;
		}

		return &MainMenu;
	}
}
