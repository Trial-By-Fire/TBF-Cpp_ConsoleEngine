#pragma once



// Includes

#include "LAL.hpp"



// Structs

class AState
{
public:
	~AState() = default;
	
	unbound ptr<AState> GetEngineState();

	unbound void SetEngineState(ptr<AState> _state);

	virtual void Load  (void) = NULL;
	virtual void Unload(void) = NULL;
	virtual void Update(void) = NULL;
	virtual void Render(void) = NULL;
};



class State : public AState
{
	friend void PrepareModules(void);

public:

	void Set(ptr<AState> _state);

	void Load  (void) final;
	void Unload(void) final;
	void Update(void) final;
	void Render(void) final;

private:

	// Functions

	unbound void LoadModule(void);


	ptr<AState> CurrentState = nullptr;
};



