#pragma once


#include "Config.hpp"
#include "LAL.hpp"
#include "Windows_Platform.hpp"
#include "State.hpp"
#include "Renderer.hpp"
#include "Cycler.hpp"
#include "Input.hpp"
#include "Memory.hpp"




namespace Engine
{
	// Functions

	// Game must implement this function.
	AState* LoadGame(void);
}
