#pragma once


// Includes

#include "Engine/Engine.hpp"



namespace Game
{
	class LevelState : public AState
	{
	public:

		static ptr<AState> Get(void);

		static void Set(ptr<AState> State);

		void Load  (void) final;
		void Unload(void) final;
		void Update(void) final;
		void Render(void) final;
	};
}

