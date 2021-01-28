// Parent Headers
#include "Game_LevelState_Ingame.hpp"



// Includes

#include "Game_EntryState.hpp"
#include "Game_LevelState_Paused.hpp"
#include "Game_Util.hpp"



namespace Game
{
	// Static Data

	// Private

	StateObj Ingame_State;

	UI_Text Ingame_Text;

	UI_Text Ingame_GameOver_Fell;

	UI_Text Ingame_GameOver_MadeIt;

	Character Player;

	Level Level1;

	Level Level2;

	sInt CurrentLevel = 1;

	bool ShouldReload = true;

	bool Ingame_DoneOnce = false;



	// Functions

	// Class Public

	void LevelState_Ingame_OnKeyEnter(Input::EState _state)
	{
		switch (_state)
		{
			case Input::EState::Pressed:
			{
				LevelState_SetSubstate(GetPausedState());

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

	void Load_Ingame(void)
	{
		if (! Ingame_DoneOnce)
		{
			COORD 
				start = { 1, 1 }, 
				end   = { 1, 1 } ;

			UI_Text_Create
			(
				&Ingame_Text,

				(WideChar*)L"Press enter to pause.\0",
				start, end,
				false
			);

			start.X = 0; start.Y = 9;
			end  .X = 0; end  .Y = 9;
			
			UI_Text_Create
			(
				&Ingame_GameOver_Fell,

				(WideChar*)L"Game Over: You fell\0",
				start, end,
				true
			);

			UI_Text_Create
			(
				&Ingame_GameOver_MadeIt,

				(WideChar*)L"Game Over: You Made It!\0",
				start, end,
				true
			);

			// Level 1

			start.X = 0;                          start.Y = Renderer::GameEnd - 1;
			end  .X = Renderer::BufferWidth - 70; end  .Y = Renderer::GameEnd - 1;

			Level_SetCells(&Level1, start, end, ELevelCell::Ground);

			start.X = 0;                    start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth - 70; end  .Y = Renderer::GameEnd;

			Level_SetCells(&Level1, start, end, ELevelCell::Ground);

			start.X = Renderer::BufferWidth - 70; start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth - 60; end  .Y = Renderer::GameEnd;

			Level_SetCells(&Level1, start, end, ELevelCell::Pit);

			start.X = Renderer::BufferWidth - 60; start.Y = Renderer::GameEnd -1;
			end  .X = Renderer::BufferWidth - 40; end  .Y = Renderer::GameEnd -1;

			Level_SetCells(&Level1, start, end, ELevelCell::Ground);

			start.X = Renderer::BufferWidth - 60; start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth - 40; end  .Y = Renderer::GameEnd;

			Level_SetCells(&Level1, start, end, ELevelCell::Ground);

			start.X = Renderer::BufferWidth - 40; start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth - 20; end  .Y = Renderer::GameEnd;

			Level_SetCells(&Level1, start, end, ELevelCell::Pit);

			start.X = Renderer::BufferWidth - 20; start.Y = Renderer::GameEnd - 1;
			end  .X = Renderer::BufferWidth;      end  .Y = Renderer::GameEnd - 1;

			Level_SetCells(&Level1, start, end, ELevelCell::Ground);

			start.X = Renderer::BufferWidth - 20; start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth;      end  .Y = Renderer::GameEnd;

			Level_SetCells(&Level1, start, end, ELevelCell::Ground);

			// Level 2

			start.X = 0;                          start.Y = Renderer::GameEnd - 1;
			end  .X = Renderer::BufferWidth - 40; end  .Y = Renderer::GameEnd - 1;

			Level_SetCells(&Level2, start, end, ELevelCell::Ground);

			start.X = 0;                          start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth - 40; end  .Y = Renderer::GameEnd;

			Level_SetCells(&Level2, start, end, ELevelCell::Ground);

			start.X = Renderer::BufferWidth - 40; start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth - 25; end  .Y = Renderer::GameEnd;

			Level_SetCells(&Level2, start, end, ELevelCell::Pit);

			start.X = Renderer::BufferWidth - 25; start.Y = Renderer::GameEnd -1;
			end  .X = Renderer::BufferWidth - 20; end  .Y = Renderer::GameEnd -1;

			Level_SetCells(&Level2, start, end, ELevelCell::Ground);

			start.X = Renderer::BufferWidth - 25; start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth - 20; end  .Y = Renderer::GameEnd;

			Level_SetCells(&Level2, start, end, ELevelCell::Ground);
		 
			start.X = Renderer::BufferWidth - 20; start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth - 10; end  .Y = Renderer::GameEnd;

			Level_SetCells(&Level2, start, end, ELevelCell::Pit);

			start.X = Renderer::BufferWidth - 10; start.Y = Renderer::GameEnd - 1;
			end  .X = Renderer::BufferWidth;      end  .Y = Renderer::GameEnd - 1;

			Level_SetCells(&Level2, start, end, ELevelCell::Finish);

			start.X = Renderer::BufferWidth - 10; start.Y = Renderer::GameEnd;
			end  .X = Renderer::BufferWidth;      end  .Y = Renderer::GameEnd;

			Level_SetCells(&Level2, start, end, ELevelCell::Finish);


			Ingame_DoneOnce = true;
		}

		if (ShouldReload == true)
		{
			Character_Load(&Player);

			CurrentLevel = 1;

			ShouldReload = false;
		}

		Input::SubscribeTo(EKeyCode::Enter      , &LevelState_Ingame_OnKeyEnter);
		Input::SubscribeTo(EKeyCode::Arrow_Up   , &LevelState_Ingame_OnKeyUp   );
		Input::SubscribeTo(EKeyCode::Arrow_Left , &LevelState_Ingame_OnKeyLeft );
		Input::SubscribeTo(EKeyCode::Arrow_Right, &LevelState_Ingame_OnKeyRight);
	}

	void Unload_Ingame(void)
	{
		Input::Unsubscribe(EKeyCode::Enter      , &LevelState_Ingame_OnKeyEnter);
		Input::Unsubscribe(EKeyCode::Arrow_Up   , &LevelState_Ingame_OnKeyUp   );
		Input::Unsubscribe(EKeyCode::Arrow_Left , &LevelState_Ingame_OnKeyLeft );
		Input::Unsubscribe(EKeyCode::Arrow_Right, &LevelState_Ingame_OnKeyRight);
	}

	void Update_Ingame(void)
	{
		if (CurrentLevel == 2 && ! Character_AtFinish(&Player, &Level2))
		{
			Character_Update(&Player, &Level2);	
		}

		if (CurrentLevel == 1)
		{
			Character_Update(&Player, &Level1);
		}

		if (CurrentLevel == 1 && Player.Position.X >= 0.98f)
		{
			CurrentLevel = 2;

			Player.Position.X = -0.975f; 
		}
	}

	void Render_Ingame(void)
	{
		if (CurrentLevel == 1)
		{
			Level_Render(&Level1);
		}
		else
		{
			Level_Render(&Level2);
		}

		UI_Text_Render(&Ingame_Text);

		Character_Render(&Player);	

		if (Player.Fell)
		{
			UI_Text_Render(&Ingame_GameOver_Fell);
		}

		if (CurrentLevel == 2 && Character_AtFinish(&Player, &Level2))
		{
			UI_Text_Render(&Ingame_GameOver_MadeIt);
		}
	}



	// Public

	StateObj* GetIngameState(void)
	{
		unbound bool stateConstructed = false;

		if (!stateConstructed)
		{
			Ingame_State.Load   = &Load_Ingame  ;
			Ingame_State.Unload = &Unload_Ingame;
			Ingame_State.Update = &Update_Ingame;
			Ingame_State.Render = &Render_Ingame;

			stateConstructed = true;
		}

		return &Ingame_State;
	}

	void Ingame_Reload(void)
	{
		ShouldReload = true;
	}
}
