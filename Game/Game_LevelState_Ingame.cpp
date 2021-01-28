// Parent Headers
#include "Game_LevelState_Ingame.hpp"



// Includes

#include "Game_EntryState.hpp"
#include "Game_LevelState_Paused.hpp"
#include "Game_Util.hpp"



namespace Game
{
	class Ingame : public AState
	{
	public:

		override void Load  (void);
		override void Unload(void);
		override void Update(void);
		override void Render(void);
	};



	// Static Data

	// Private

	Ingame Ingame_State;

	UI_Text Ingame_Text;

	UI_Text Ingame_GameOver_Fell;

	UI_Text Ingame_GameOver_MadeIt;

	Character Player;

	Level Level1;

	Level Level2;

	sInt CurrentLevel = 1;

	bool ShouldReload = false;

	bool Ingame_DoneOnce = false;



	// Functions

	// Class Public

	void LevelState_Ingame_OnKeyEnter(Input::EState _state)
	{
		switch (_state)
		{
			case Input::EState::Pressed:
			{
				LevelState::Set(GetPausedState());

				break;
			}
		}
	}

	void LevelState_Ingame_OnKeyUp(Input::EState _state)
	{
		switch (_state)
		{
			case Input::EState::Pressed:
			{
				Player.ShouldJump = true;

				break;
			}
			case Input::EState::Released:
			{
				break;
			}
		}
	}

	void LevelState_Ingame_OnKeyLeft(Input::EState _state)
	{
		switch (_state)
		{
			case Input::EState::Pressed:
			{
				Player.MoveState = ECharacter_Move::Left;

				break;
			}
			case Input::EState::Released:
			{
				if (Player.MoveState == ECharacter_Move::Left)
				{
					Player.MoveState = ECharacter_Move::Dont;
				}

				break;
			}
		}
	}

	void LevelState_Ingame_OnKeyRight(Input::EState _state)
	{
		switch (_state)
		{
			case Input::EState::Pressed:
			{
				Player.MoveState = ECharacter_Move::Right;

				break;
			}
			case Input::EState::Released:
			{
				if (Player.MoveState == ECharacter_Move::Right)
				{
					Player.MoveState = ECharacter_Move::Dont;
				}

				break;
			}
		}
	}

	void Ingame::Load(void)
	{
		if (! Ingame_DoneOnce)
		{
			COORD 
				start = { 1, 1 }, 
				end   = { 1, 1 } ;

			Ingame_Text.Create
			(
				(WideChar*)L"Press enter to pause.\0",
				start, end,
				false
			);

			start.X = 0; start.Y = 9;
			end  .X = 0; end  .Y = 9;
			
			Ingame_GameOver_Fell.Create
			(
				(WideChar*)L"Game Over: You fell\0",
				start, end,
				true
			);

			Ingame_GameOver_MadeIt.Create
			(
				(WideChar*)L"Game Over: You Made It!\0",
				start, end,
				true
			);

			// Level 1

			start.X = 0;                          start.Y = Renderer::GameEnd - 1;
			end  .X = Renderer::BufferWidth - 70; end  .Y = Renderer::GameEnd - 1;

			Level1.SetCells(start, end, ELevelCell::Ground);

			start.X = 0;                    start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth - 70; end  .Y = Renderer::GameEnd;

			Level1.SetCells(start, end, ELevelCell::Ground);

			start.X = Renderer::BufferWidth - 70; start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth - 60; end  .Y = Renderer::GameEnd;

			Level1.SetCells(start, end, ELevelCell::Pit);

			start.X = Renderer::BufferWidth - 60; start.Y = Renderer::GameEnd -1;
			end  .X = Renderer::BufferWidth - 40; end  .Y = Renderer::GameEnd -1;

			Level1.SetCells(start, end, ELevelCell::Ground);

			start.X = Renderer::BufferWidth - 60; start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth - 40; end  .Y = Renderer::GameEnd;

			Level1.SetCells(start, end, ELevelCell::Ground);

			start.X = Renderer::BufferWidth - 40; start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth - 20; end  .Y = Renderer::GameEnd;

			Level1.SetCells(start, end, ELevelCell::Pit);

			start.X = Renderer::BufferWidth - 20; start.Y = Renderer::GameEnd - 1;
			end  .X = Renderer::BufferWidth;      end  .Y = Renderer::GameEnd - 1;

			Level1.SetCells(start, end, ELevelCell::Ground);

			start.X = Renderer::BufferWidth - 20; start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth;      end  .Y = Renderer::GameEnd;

			Level1.SetCells(start, end, ELevelCell::Ground);

			// Level 2

			start.X = 0;                          start.Y = Renderer::GameEnd - 1;
			end  .X = Renderer::BufferWidth - 40; end  .Y = Renderer::GameEnd - 1;

			Level2.SetCells(start, end, ELevelCell::Ground);

			start.X = 0;                          start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth - 40; end  .Y = Renderer::GameEnd;

			Level2.SetCells(start, end, ELevelCell::Ground);

			start.X = Renderer::BufferWidth - 40; start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth - 25; end  .Y = Renderer::GameEnd;

			Level2.SetCells(start, end, ELevelCell::Pit);

			start.X = Renderer::BufferWidth - 25; start.Y = Renderer::GameEnd -1;
			end  .X = Renderer::BufferWidth - 20; end  .Y = Renderer::GameEnd -1;

			Level2.SetCells(start, end, ELevelCell::Ground);

			start.X = Renderer::BufferWidth - 25; start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth - 20; end  .Y = Renderer::GameEnd;

			Level2.SetCells(start, end, ELevelCell::Ground);
		 
			start.X = Renderer::BufferWidth - 20; start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth - 10; end  .Y = Renderer::GameEnd;

			Level2.SetCells(start, end, ELevelCell::Pit);

			start.X = Renderer::BufferWidth - 10; start.Y = Renderer::GameEnd - 1;
			end  .X = Renderer::BufferWidth;      end  .Y = Renderer::GameEnd - 1;

			Level2.SetCells(start, end, ELevelCell::Finish);

			start.X = Renderer::BufferWidth - 10; start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth;      end  .Y = Renderer::GameEnd;

			Level2.SetCells(start, end, ELevelCell::Finish);


			Ingame_DoneOnce = true;
		}

		if (ShouldReload == true)
		{
			Player.Load();

			CurrentLevel = 1;

			ShouldReload = false;
		}

		Input::SubscribeTo(EKeyCode::Enter      , LevelState_Ingame_OnKeyEnter);
		Input::SubscribeTo(EKeyCode::Arrow_Up   , LevelState_Ingame_OnKeyUp   );
		Input::SubscribeTo(EKeyCode::Arrow_Left , LevelState_Ingame_OnKeyLeft );
		Input::SubscribeTo(EKeyCode::Arrow_Right, LevelState_Ingame_OnKeyRight);
	}

	void Ingame::Unload(void)
	{
		Input::Unsubscribe(EKeyCode::Enter      , LevelState_Ingame_OnKeyEnter);
		Input::Unsubscribe(EKeyCode::Arrow_Up   , LevelState_Ingame_OnKeyUp   );
		Input::Unsubscribe(EKeyCode::Arrow_Left , LevelState_Ingame_OnKeyLeft );
		Input::Unsubscribe(EKeyCode::Arrow_Right, LevelState_Ingame_OnKeyRight);
	}

	void Ingame::Update(void)
	{
		if (CurrentLevel == 2 && ! Player.AtFinish(Level2))
		{
			Player.Update(Level2);	
		}

		if (CurrentLevel == 1)
		{
			Player.Update(Level1);
		}

		if (CurrentLevel == 1 && Player.Position.X >= 0.98f)
		{
			CurrentLevel = 2;

			Player.Position.X = -0.975f; 
		}
	}

	void Ingame::Render(void)
	{
		if (CurrentLevel == 1)
		{
			Level1.Render();
		}
		else
		{
			Level2.Render();
		}

		Ingame_Text.Render();

		Player.Render();	

		if (Player.Fell)
		{
			Ingame_GameOver_Fell.Render();
		}

		if (CurrentLevel == 2 && Player.AtFinish(Level2))
		{
			Ingame_GameOver_MadeIt.Render();
		}
	}



	// Public

	AState* GetIngameState(void)
	{
		return &Ingame_State;
	}

	void Ingame_Reload(void)
	{
		ShouldReload = true;
	}
}
