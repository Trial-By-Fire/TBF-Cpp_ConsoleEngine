// Parent Header
#include "Game_EntryState.hpp"



// Include

#include "Game_LevelState.hpp"
#include "Game_Util.hpp"



namespace Game
{
	class MainMenu : public AState
	{
	public:

		static void PressStart(void);
		static void PressQuit (void);

		void Load  (void) final;
		void Unload(void) final;
		void Update(void) final;
		void Render(void) final;
	};



	// Static Data

	// Private

	MainMenu StateObj;

	UI_Widget MenuWidget;

	bool Menu_DoneOnce = false;



	// Functions

	// Public Class

	void MainMenu::PressStart(void)
	{
		WriteToLog(L"UI Start Selected");

		State::SetEngineState(LevelState::Get());
	}

	void MainMenu::PressQuit(void)
	{
		Cycler::Lapse();
	}

	void Game_EntryState_OnKeyArrowUp(Input::EState _state)
	{
		switch (_state)
		{
			case Input::EState::Pressed:
			{
				WriteToLog(L"EntryState: On Key Up");

				MenuWidget.MoveUp();

				break;
			}
		}
	}

	void Game_EntryState_OnKeyArrowDown(Input::EState _state)
	{
		switch (_state)
		{
			case Input::EState::Pressed:
			{
				WriteToLog(L"EntryState: On Key Down");

				MenuWidget.MoveDown();

				break;
			}
		}
	}

	void Game_EntryState_OnKeyEnter(Input::EState _state)
	{
		switch (_state)
		{
			case Input::EState::Pressed:
			{
				WriteToLog(L"EntryState: On Key Enter");

				MenuWidget.Select();

				break;
			}
		}
	}

	void MainMenu::Load(void)
	{
		if (! Menu_DoneOnce)
		{
			COORD startCell, endCell;

			startCell.X = 0; endCell.X = 0;
			startCell.Y = 9; endCell.Y = 9;


			MenuWidget.AddText
			(
				L"Generic Platformer Demo",
				startCell, 
				endCell,
				true   // Should Center
			);

			startCell.Y = 15; endCell.Y = 15;

			MenuWidget.AddButton
			(
				L"Start",
				startCell, endCell,
				true,
				MainMenu::PressStart
			);

			startCell.X = -1; endCell.X = -1;
			startCell.Y = 17; endCell.Y = 17;

			MenuWidget.AddButton
			(
				L"Quit",
				startCell, endCell,
				true,
				MainMenu::PressQuit
			);

			Menu_DoneOnce = true;
		}

		Input::SubscribeTo(EKeyCode::Arrow_Up  , Game_EntryState_OnKeyArrowUp  );
		Input::SubscribeTo(EKeyCode::Arrow_Down, Game_EntryState_OnKeyArrowDown);
		Input::SubscribeTo(EKeyCode::Enter     , Game_EntryState_OnKeyEnter    );

		WriteToLog(L"Loaded: Main Menu");
	}

	void MainMenu::Unload(void)
	{
		Input::Unsubscribe(EKeyCode::Arrow_Up  , Game_EntryState_OnKeyArrowUp  );
		Input::Unsubscribe(EKeyCode::Arrow_Down, Game_EntryState_OnKeyArrowDown);
		Input::Unsubscribe(EKeyCode::Enter     , Game_EntryState_OnKeyEnter    );

		WriteToLog(L"Unload: Main Menu");
	}

	void MainMenu::Update(void)
	{
	}

	void MainMenu::Render(void)
	{
		MenuWidget.Render();
	}
}


namespace Engine
{
	// Engine Entrypoint

	ptr<AState> LoadGame(void)
	{
		return getPtr(Game::StateObj);
	}
}
