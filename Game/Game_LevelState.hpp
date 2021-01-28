#pragma once


// Includes

#include "Engine/Engine.hpp"



namespace Game
{
	// Functions

	State::Callbacks* GetLevelState(void);

	void LevelState_SetSubstate(State::Callbacks* _state);
}

