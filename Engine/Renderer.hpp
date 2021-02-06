#pragma once



// Includes

#include "Config.hpp"
#include "LAL.hpp"
#include "OSPlatform.hpp"
#include "Timing.hpp"




class Renderer
{
	friend Timing;

public:

	// Constants

	unbound CompileTime uInt16 BufferWidth     = 80;
	unbound CompileTime uInt16 BufferHeight    = DebugEnabled ? 48 : 24;
	unbound CompileTime uInt16 BorderLineRow   = 24;
	unbound CompileTime uInt16 DebugStart      = 25;
	unbound CompileTime uInt16 LogSize         = 18;
	unbound CompileTime uInt16 PersistentStart = 44;
	unbound CompileTime uInt16 PersistentSize  = 4 ;
	unbound CompileTime uInt16 GameEnd         = 23;



	// Aliases (Typedefs)

	using CAttribute = OSPlatform::CAttribute;
	using Cell       = OSPlatform::ConsoleCell;
	using CSBI       = CONSOLE_SCREEN_BUFFER_INFO;
	using Line       = StaticArray<Cell, BufferWidth>;



	// Structures

	struct Vec2D_Int 
	{ 
		sInt 
			X = 0, 
			Y = 0 ; 
	};

	struct Data
	{
		// Console Data

		HWND   Window_Handle = nullptr;
		HANDLE Output_Handle = OSPlatform::StdHandle::Invalid();

		DWORD      CharactersWritten = 0;
		COORD      CoordSize         = { BufferWidth, BufferHeight };
		CSBI       CSBI_Instance       {};

		DWORD      BufferSize        = 0;
		SMALL_RECT Num               = { 0, 0};

		CONSOLE_CURSOR_INFO CursorSettings {};

		Vec2D_Int ScreenPosition {};

		Timer RefreshTimer = Timer(1.0 / 60.0);
	};

	struct ScreenInfo
	{
		Vec2D_Int Center;
	};



	// Functions

	unbound ro Data& GetContext(void);

	unbound void LoadModule(void);

	unbound void UnloadModule(void);

	unbound void Update(void);

	unbound void WriteToBufferCells(ptr<Cell> _cells, COORD _initalCell, COORD _finalCell);

	unbound void WriteToLog(WString _logString);

	unbound void WriteToPersistentSection(sInt _row, WString _lineformat, ...);

	unbound void Logs_ScrollUp(void);

	unbound void Logs_ScrollDown(void);


private:

	unbound void DrawGameScanlines    (void);
	unbound void InitalizeData        (void);
	unbound void SetupConsole         (void);
	unbound bool UpdateConsoleInfo    (void);
	unbound void UpdateSizeAndPosition(void);

	unbound void Clear(void);

	unbound bool FillCellsWithWhitespace(void);

	unbound void ProcessTiming();

	unbound bool FormatCells(void);

	unbound void RenderFrame(void);

	unbound void ResetDrawPosition(void);


	unbound Data Context;
};

