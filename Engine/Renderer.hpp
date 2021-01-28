#pragma once



// Includes

#include "Config.hpp"
#include "LAL.hpp"
#include "CString.hpp"
#include "OSPlatform.hpp"




namespace Renderer
{
	// Constants

	CompileTime uInt16 BufferWidth     = 80;
	CompileTime uInt16 BufferHeight    = DebugEnabled ? 48 : 24;
	CompileTime uInt16 BorderLineRow   = 24;
	CompileTime uInt16 DebugStart      = 25;
	CompileTime uInt16 LogSize         = 18;
	CompileTime uInt16 PersistentStart = 44;
	CompileTime uInt16 PersistentSize  = 4 ;
	CompileTime uInt16 GameEnd         = 23;



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

		// Render Timing

		float64 RefeshTimer   ;
		float64 RefeshInterval;
	};

	struct ScreenInfo
	{
		Vec2D_Int Center;
	};



	// Functions

	void Clear(void);

	bool FillCellsWithWhitespace(void);

	bool FormatCells(void);

	ro Data* GetContext(void);

	void LoadModule(void);

	void ProcessTiming(float64 _deltaTime);

	void RenderFrame(void);

	void ResetDrawPosition(void);

	void UnloadModule(void);

	void Update(void);

	void WriteToBufferCells(Cell* _cells, COORD _initalCell, COORD _finalCell);

	// BS Fix for now:
	using WideChar = wchar_t;   // From C_String.h

	void WriteToLog(WideChar* _logString);

	void WriteToPersistentSection(sInt _row, WideChar* _lineformat, ...);

	void Logs_ScrollUp(void);

	void Logs_ScrollDown(void);
}

