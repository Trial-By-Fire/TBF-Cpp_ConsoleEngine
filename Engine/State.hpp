#pragma once



// Includes

#include "LAL.hpp"



// Structs

class State
{
public:

	struct Callbacks
	{
		Void_Function* Load;
		Void_Function* Unload;
		Void_Function* Update;
		Void_Function* Render;
	};


	// Functions

	unbound void LoadModule(void);

	unbound void LoadGame(void);

	unbound ro State* GetEngineState();

	unbound void SetEngineState(Callbacks* _state);

	void Set(Callbacks* _state);

	void Unload(void) ro;

	void Update(void) ro;

	void Render(void) ro;

private:

	Callbacks* data;
};



