#pragma once



// Includes

#include "Config.hpp"
#include "LAL.hpp"
#include "CString.hpp"
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

	using CSBI = CONSOLE_SCREEN_BUFFER_INFO;
	using Cell = CHAR_INFO;
	using Line = Cell[BufferWidth];



	// Structures

	struct Vec2D_Int { sInt X, Y; };

	struct Data
	{
		// Console Data

		HWND   Window_Handle;
		HANDLE Output_Handle;

		DWORD      CharactersWritten;
		COORD      CoordSize;
		CSBI       CSBI_Instance;
		DWORD      BufferSize;
		SMALL_RECT Size;

		CONSOLE_CURSOR_INFO CursorSettings;

		Vec2D_Int ScreenPosition;

		Timer RefreshTimer;
	};

	struct ScreenInfo
	{
		Vec2D_Int Center;
	};



	// Functions

	unbound ro Data* GetContext(void);

	unbound void LoadModule(void);

	unbound void UnloadModule(void);

	unbound void Update(void);

	unbound void WriteToBufferCells(Cell* _cells, COORD _initalCell, COORD _finalCell);

	// BS Fix for now:
	using WideChar = wchar_t;   // From C_String.h

	unbound void WriteToLog(WideChar* _logString);

	unbound void WriteToPersistentSection(sInt _row, WideChar* _lineformat, ...);

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

	unbound void ProcessTiming(float64 _deltaTime);

	unbound bool FormatCells(void);

	unbound void RenderFrame(void);

	unbound void ResetDrawPosition(void);


	unbound Data Context;
};

