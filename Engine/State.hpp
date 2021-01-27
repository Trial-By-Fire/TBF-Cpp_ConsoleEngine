#pragma once



// Includes

#include "LAL.hpp"



// Structs

struct StateObj
{
	Void_Function* Load;
	Void_Function* Unload;
	Void_Function* Update;
	Void_Function* Render;
};



namespace State
{
	// Aliases (Typedefs)



	// Functions

	void LoadModule(void);

	void SetState(StateObj* _state);

	void Update(void);

	void Render(void);

	void LoadGame(void);
}
