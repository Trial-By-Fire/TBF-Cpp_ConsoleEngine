// Parent Header
#include "Game_LevelState.hpp"



// Includes

#include "Game_EntryState.hpp"
#include "Game_LevelState_Ingame.hpp"
#include "Game_Util.hpp"



namespace Game
{
	// Static Data

	// Private

	State::Callbacks LevelState;

	State::Callbacks* LevelState_CurrentSubstate = nullptr;

	State Substate;



	// Functions

	// Class Public

	void Load_Level(void)
	{
		Substate.Set(GetIngameState());
	}

	void Unload_Level(void)
	{
		Substate.Unload();
	}

	void Update_Level(void)
	{
		Substate.Update();
	}

	void Render_Level(void)
	{
		Substate.Render();
	}



	// Public

	State::Callbacks* GetLevelState(void)
	{
		unbound bool stateConstructed = false;

		if (! stateConstructed)
		{
			LevelState.Load   = &Load_Level  ;
			LevelState.Unload = &Unload_Level;
			LevelState.Update = &Update_Level;
			LevelState.Render = &Render_Level;

			stateConstructed = true;
		}

		return &LevelState;
	}

	void LevelState_SetSubstate(State::Callbacks* _state)
	{
		Substate.Set(_state);
	}
}

