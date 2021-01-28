#pragma once



// Include

#include "Game_LevelState.hpp"



namespace Game
{
	State::Callbacks* GetIngameState(void);

	void Ingame_Reload(void);
}

