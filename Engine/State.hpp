#pragma once



// Includes

#include "LAL.hpp"



// Structs

class AState
{
public:
	~AState() {};
	
	unbound AState* GetEngineState();

	unbound void SetEngineState(AState* _state);

	virtual void Load  (void) = NULL;
	virtual void Unload(void) = NULL;
	virtual void Update(void) = NULL;
	virtual void Render(void) = NULL;
};



class State : public AState
{
	friend void PrepareModules(void);

public:

	void Set(AState* _state);

	override void Load  (void);
	override void Unload(void);
	override void Update(void);
	override void Render(void);

private:

	// Functions

	unbound void LoadModule(void);


	AState* CurrentState = nullptr;
};



