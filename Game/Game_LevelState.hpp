#pragma once


// Includes

#include "Engine/Engine.hpp"



namespace Game
{
	// Functions

	StateObj* GetLevelState(void);

	void LevelState_SetSubstate(StateObj* _state);
}

