#pragma once



// Include

#include "Game_LevelState.hpp"



namespace Game
{
	ptr<AState> GetIngameState(void);

	void Ingame_Reload(void);
}

