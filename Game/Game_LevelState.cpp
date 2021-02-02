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

	LevelState StateObj;

	State Substate;



	// Functions

	ptr<AState> LevelState::Get(void)
	{
		return getPtr(StateObj);
	}

	void LevelState::Set(ptr<AState> _state)
	{
		Substate.Set(_state);
	}

	// AState

	void LevelState::Load(void)
	{
		Substate.Set(GetIngameState());
	}

	void LevelState::Unload(void)
	{
		Substate.Unload();
	}

	void LevelState::Update(void)
	{
		Substate.Update();
	}

	void LevelState::Render(void)
	{
		Substate.Render();
	}
}

