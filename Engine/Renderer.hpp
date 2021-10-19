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

	static constexpr uInt16 BufferWidth     = 80;
	static constexpr uInt16 BufferHeight    = DebugEnabled ? 48 : 24;
	static constexpr uInt16 BorderLineRow   = 24;
	static constexpr uInt16 DebugStart      = 25;
	static constexpr uInt16 LogSize         = 18;
	static constexpr uInt16 PersistentStart = 44;
	static constexpr uInt16 PersistentSize  = 4 ;
	static constexpr uInt16 GameEnd         = 23;



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

	static const Data& GetContext(void);

	static void LoadModule(void);

	static void UnloadModule(void);

	static void Update(void);

	static void WriteToBufferCells(ptr<Cell> _cells, COORD _initalCell, COORD _finalCell);

	static void WriteToLog(WString _logString);

	static void WriteToPersistentSection(sInt _row, WString _lineformat, ...);

	static void Logs_ScrollUp(void);

	static void Logs_ScrollDown(void);


private:

	static void DrawGameScanlines    (void);
	static void InitalizeData        (void);
	static void SetupConsole         (void);
	static bool UpdateConsoleInfo    (void);
	static void UpdateSizeAndPosition(void);

	static void Clear(void);

	static bool FillCellsWithWhitespace(void);

	static void ProcessTiming();

	static bool FormatCells(void);

	static void RenderFrame(void);

	static void ResetDrawPosition(void);


	static Data Context;
};

