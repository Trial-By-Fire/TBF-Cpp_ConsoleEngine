#pragma once


// Includes

#include "Engine/Engine.hpp"



namespace Game
{
	class LevelState : public AState
	{
	public:

		unbound ptr<AState> Get(void);

		unbound void Set(ptr<AState> State);

		void Load  (void) final;
		void Unload(void) final;
		void Update(void) final;
		void Render(void) final;
	};
}

