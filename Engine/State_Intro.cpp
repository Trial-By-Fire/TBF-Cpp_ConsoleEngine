// Parent Header
#include "State_Intro.hpp"



// Include
#include "Renderer.hpp"
#include "Cycler.hpp"
#include "Timing.hpp"
#include "Engine.hpp"



namespace Intro
{
	using OSPlatform::Console_WhiteCell;


	class IntroState : public AState
	{
	public:

		void Load  (void) final;
		void Unload(void) final;
		void Update(void) final;
		void Render(void) final;
	};


	IntroState StateObj;

	Timer IntroTimer(7.0);

	Timer Timer_TillTitle          (2.0);
	Timer Timer_TillVersion        (1.2);
	Timer Timer_TillIntroFadeToGrey(Timer_TillTitle.EndTime.count() + 4.2);

	Timer Timer_TillTitle_ToWhite  (0.134);
	Timer Timer_TillVersion_ToWhite(0.134);

	Timer Timer_Till_FadeOut(0.134);


	bool Intro_DoneOnce = false;

	WString IntroTitle    = L"Trial By Fire Engine";
	WString EngineVersion = L"Type C++"            ;

	bool 
		RenderTitle   = false, 
		RenderVersion = false ;

	DynamicArray<Renderer::Cell> IntroTitle_RenderCells;
	DynamicArray<Renderer::Cell> Version_RenderCells   ;



	// Functions

	// Private

	void ChangeTo_Grey(DynamicArray<Renderer::Cell>& _cells)
	{
		for (auto& cell : _cells)
		{
			cell.Attributes.Set(Renderer::CAttribute::FG_Intensity);
		}
	}

	void ChangeTo_White(DynamicArray<Renderer::Cell>& _cells)
	{
		for (auto& cell : _cells)
		{
			cell.Attributes = Console_WhiteCell;
		}
	}


	// Class Public

	void IntroState::Load(void)
	{
		Renderer::WriteToLog(L"Intro State: Loaded");

		if (! Intro_DoneOnce)
		{
			using Cell = Renderer::Cell;

			IntroTitle_RenderCells.resize(IntroTitle   .size());
			Version_RenderCells   .resize(EngineVersion.size());

			for (uIntDM cellIndex = 0; cellIndex < IntroTitle_RenderCells.size(); cellIndex++)
			{
				IntroTitle_RenderCells[cellIndex].Char = IntroTitle[cellIndex];
			}

			for (uIntDM cellIndex = 0; cellIndex < Version_RenderCells.size(); cellIndex++)
			{
				Version_RenderCells[cellIndex].Char = EngineVersion[cellIndex];
			}

			Intro_DoneOnce = true;
		}

		ChangeTo_Grey(IntroTitle_RenderCells);
		ChangeTo_Grey(Version_RenderCells   );
	}

	void IntroState::Unload(void)
	{
		Renderer::WriteToLog(L"Intro State: Unloaded");
	}

	void IntroState::Update(void)
	{
		unbound CompileTime auto WriteToLog               =  Renderer::WriteToLog;
		unbound CompileTime auto WriteToPersistentSection =  Renderer::WriteToPersistentSection;


		unbound bool 
			LogTitle   = true, 
			LogVersion = true, 
			LogFade    = true, 
			LogEnd     = true,

			Log_ChangeToWhite_Title   = true,
			Log_ChangeToWhite_Version = true,

			Log_FadeToGrey = true;


		IntroTimer     .Tick();
		Timer_TillTitle.Tick();

		WriteToPersistentSection(4, WString(L"Intro Time Elapsed: ") + ToWString(IntroTimer.Elapsed.count()));

		if (Timer_TillTitle.Ended())
		{
			if (LogTitle)
			{
				WriteToLog(L"Title should show up now (Grey).");

				RenderTitle = true;

				LogTitle = false;
			}

			Timer_TillTitle_ToWhite.Tick();

			if (Log_ChangeToWhite_Title &&  Timer_TillTitle_ToWhite.Ended())
			{
				ChangeTo_White(IntroTitle_RenderCells);

				WriteToLog(L"Title: White attribute set.");

				Log_ChangeToWhite_Title = false;
			}

			Timer_TillVersion.Tick();

			if (LogVersion && Timer_TillVersion.Ended())
			{
				WriteToLog(L"Version should show up now.");

				RenderVersion = true;

				LogVersion = false;
			}

			if (RenderVersion)
			{
				Timer_TillVersion_ToWhite.Tick();

				if (Log_ChangeToWhite_Version && Timer_TillVersion_ToWhite.Ended())
				{
					ChangeTo_White(Version_RenderCells);

					WriteToLog(L"Engine Version: White attribute set.");

					Log_ChangeToWhite_Version = false;
				}
			}
		}

		Timer_TillIntroFadeToGrey.Tick();

		if (Timer_TillIntroFadeToGrey.Ended())
		{
			ChangeTo_Grey(IntroTitle_RenderCells);
			ChangeTo_Grey(Version_RenderCells   );

			if (Log_FadeToGrey)
			{
				WriteToLog(L"Title should fade to gray now.");

				Log_FadeToGrey = false;
			}

			Timer_Till_FadeOut.Tick();

			if (LogFade && Timer_Till_FadeOut.Ended())
			{
				WriteToLog(L"Title should fade out now.");

				RenderTitle   = false;
				RenderVersion = false;

				LogFade = false;
			}
		}

		if (LogEnd && IntroTimer.Ended() )
		{
			WriteToLog(L"Intro Ends now.");

			LogEnd = false;

			State::SetEngineState(Engine::LoadGame());
		}
	}

	void IntroState::Render(void)
	{
		unbound CompileTime auto WriteToBufferCells = Renderer::WriteToBufferCells;


		unbound COORD
			startingCell = { 0, 9 },
			endingCell   = { 0, 9 };


		// Render Title
		if (RenderTitle)
		{
			startingCell.X = (Renderer::BufferWidth / 2) - (SCast<uInt16>(IntroTitle_RenderCells.size()) / 2);
			endingCell  .X = (Renderer::BufferWidth / 2) + (SCast<uInt16>(IntroTitle_RenderCells.size()) / 2);

			startingCell.Y = 9;
			endingCell  .Y = 9;

			WriteToBufferCells(IntroTitle_RenderCells.data(), startingCell, endingCell);
		}

		// Render Version
		if (RenderVersion)
		{
			startingCell.X = (Renderer::BufferWidth / 2) - (SCast<uInt16>(Version_RenderCells.size()) / 2);
			endingCell  .X = (Renderer::BufferWidth / 2) + (SCast<uInt16>(Version_RenderCells.size()) / 2);

			startingCell.Y = 11;
			endingCell  .Y = 11;

			WriteToBufferCells(Version_RenderCells.data(), startingCell, endingCell);
		}
	}



	// Public

	ptr<AState> GetState(void)
	{
		return getPtr(StateObj);
	}
}
