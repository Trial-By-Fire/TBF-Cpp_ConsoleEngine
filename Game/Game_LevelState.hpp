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

		override void Load  (void);
		override void Unload(void);
		override void Update(void);
		override void Render(void);
	};
}

