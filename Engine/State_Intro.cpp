// Parent Header
#include "State_Intro.hpp"



// Include
#include "Renderer.hpp"
#include "Cycler.hpp"
#include "Timing.hpp"



namespace Intro
{
	using OSPlatform::Console_WhiteCell;



	State::Callbacks IntroState;

	Timer IntroTimer;

	Timer Timer_TillTitle;
	Timer Timer_TillVersion;
	Timer Timer_TillIntroFadeToGrey;

	Timer Timer_TillTitle_ToWhite;
	Timer Timer_TillVersion_ToWhite;

	Timer Timer_Till_FadeOut;


	bool Intro_DoneOnce = false;

	CTS_CWString IntroTitle    = L"Trial By Fire Engine";
	CTS_CWString EngineVersion = L"Type C++"           ;

	bool 
		RenderTitle   = false, 
		RenderVersion = false ;

	Renderer::Cell* IntroTitle_RenderCells = nullptr;
	Renderer::Cell* Version_RenderCells    = nullptr;

	uIntDM 
		Title_Length         = 0,
		EngineVersion_Length = 0 ;



	// Functions

	// Private

	void ChangeTitleTo_Grey()
	{
		for (uIntDM cellIndex = 0; cellIndex < Title_Length; cellIndex++)
		{
			IntroTitle_RenderCells[cellIndex].Attributes = FOREGROUND_INTENSITY;
		}
	}

	void ChangeEngineVerTo_Grey()
	{
		for (uIntDM cellIndex = 0; cellIndex < EngineVersion_Length; cellIndex++)
		{
			Version_RenderCells[cellIndex].Attributes = FOREGROUND_INTENSITY;
		}
	}

	void ChangeTitleTo_White()
	{
		for (uIntDM cellIndex = 0; cellIndex < Title_Length; cellIndex++)
		{
			IntroTitle_RenderCells[cellIndex].Attributes = Console_WhiteCell;
		}
	}

	void ChangeEngineVerTo_White()
	{
		for (uIntDM cellIndex = 0; cellIndex < EngineVersion_Length; cellIndex++)
		{
			Version_RenderCells[cellIndex].Attributes = Console_WhiteCell;
		}
	}


	// Class Public

	void IntroState_Load(void)
	{
		Renderer::WriteToLog((WideChar*)L"Intro State: Loaded");

		if (! Intro_DoneOnce)
		{
			using Cell = Renderer::Cell;

			IntroTimer.EndTime = 7.0L;

			Timer_TillTitle    .EndTime = 2.0L;
			Timer_TillVersion  .EndTime = 1.2L;

			Timer_TillIntroFadeToGrey.EndTime = Timer_TillTitle.EndTime + 4.2L;

			Timer_Till_FadeOut.EndTime = 0.134L;

			Timer_TillTitle_ToWhite  .EndTime = 0.134L;
			Timer_TillVersion_ToWhite.EndTime = 0.134L;

			Title_Length         = wcslen(IntroTitle);
			EngineVersion_Length = wcslen(EngineVersion) + 1;

			IntroTitle_RenderCells = (Cell*)GlobalAllocate(Cell, Title_Length        );
			Version_RenderCells    = (Cell*)GlobalAllocate(Cell, EngineVersion_Length);

			for (uIntDM cellIndex = 0; cellIndex < Title_Length; cellIndex++)
			{
				IntroTitle_RenderCells[cellIndex].Char.UnicodeChar = IntroTitle[cellIndex];
			}

			for (uIntDM cellIndex = 0; cellIndex < EngineVersion_Length; cellIndex++)
			{
				Version_RenderCells[cellIndex].Char.UnicodeChar = EngineVersion[cellIndex];
			}

			Intro_DoneOnce = true;
		}

		ChangeTitleTo_Grey();

		ChangeEngineVerTo_Grey();
	}

	void IntroState_Unload(void)
	{
		Renderer::WriteToLog((WideChar*)L"Intro State: Unloaded");
	}

	void IntroState_Update(void)
	{
		unbound CompileTime auto WriteToLog               =  Renderer::WriteToLog;
		unbound CompileTime auto WriteToPersistentSection =  Renderer::WriteToPersistentSection;


		static bool 
			LogTitle   = true, 
			LogVersion = true, 
			LogFade    = true, 
			LogEnd     = true,

			Log_ChangeToWhite_Title   = true,
			Log_ChangeToWhite_Version = true,

			Log_FadeToGrey = true;


		IntroTimer.Tick();

		Timer_TillTitle.Tick();

		WriteToPersistentSection(4, (WideChar*)L"Intro Time Elapsed: %.7Lf", IntroTimer.Elapsed);

		if (Timer_TillTitle.Ended())
		{
			if (LogTitle)
			{
				WriteToLog((WideChar*)L"Title should show up now (Grey).");

				RenderTitle = true;

				LogTitle = false;
			}

			Timer_TillTitle_ToWhite.Tick();

			if (Log_ChangeToWhite_Title &&  Timer_TillTitle_ToWhite.Ended())
			{
				ChangeTitleTo_White();

				WriteToLog((WideChar*)L"Title: White attribute set.");

				Log_ChangeToWhite_Title = false;
			}

			Timer_TillVersion.Tick();

			if (LogVersion && Timer_TillVersion.Ended())
			{
				WriteToLog((WideChar*)L"Version should show up now.");

				RenderVersion = true;

				LogVersion = false;
			}

			if (RenderVersion)
			{
				Timer_TillVersion_ToWhite.Tick();

				if (Log_ChangeToWhite_Version && Timer_TillVersion_ToWhite.Ended())
				{
					ChangeEngineVerTo_White();

					WriteToLog((WideChar*)L"Engine Version: White attribute set.");

					Log_ChangeToWhite_Version = false;
				}
			}
		}

		Timer_TillIntroFadeToGrey.Tick();

		if (Timer_TillIntroFadeToGrey.Ended())
		{
			ChangeTitleTo_Grey();

			ChangeEngineVerTo_Grey();

			if (Log_FadeToGrey)
			{
				WriteToLog((WideChar*)L"Title should fade to gray now.");

				Log_FadeToGrey = false;
			}

			Timer_Till_FadeOut.Tick();

			if (LogFade && Timer_Till_FadeOut.Ended())
			{
				WriteToLog((WideChar*)L"Title should fade out now.");

				RenderTitle   = false;
				RenderVersion = false;

				LogFade = false;
			}
		}

		if (LogEnd && IntroTimer.Ended() )
		{
			WriteToLog((WideChar*)L"Intro Ends now.");

			LogEnd = false;

			State::LoadGame();
		}
	}

	void IntroState_Render(void)
	{
		unbound CompileTime auto WriteToBufferCells = Renderer::WriteToBufferCells;


		unbound COORD
			startingCell = { 0, 9 },
			endingCell   = { 0, 9 };


		// Render Title
		if (RenderTitle)
		{
			startingCell.X = (Renderer::BufferWidth / 2) - (Title_Length / 2);
			endingCell  .X = (Renderer::BufferWidth / 2) + (Title_Length / 2);

			startingCell.Y = 9;
			endingCell  .Y = 9;

			WriteToBufferCells(IntroTitle_RenderCells, startingCell, endingCell);
		}

		// Render Version
		if (RenderVersion)
		{
			startingCell.X = (Renderer::BufferWidth / 2) - (EngineVersion_Length / 2);
			endingCell  .X = (Renderer::BufferWidth / 2) + (EngineVersion_Length / 2);

			startingCell.Y = 11;
			endingCell  .Y = 11;

			WriteToBufferCells(Version_RenderCells, startingCell, endingCell);
		}
	}



	// Public

	State::Callbacks* GetState(void)
	{
		unbound bool firstGet = true;

		if (firstGet) 
		{
			IntroState.Load   = &IntroState_Load  ;
			IntroState.Unload = &IntroState_Unload;
			IntroState.Update = &IntroState_Update;
			IntroState.Render = &IntroState_Render;

			firstGet = false;
		}

		return &IntroState;
	}
}
