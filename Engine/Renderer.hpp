#pragma once



// Includes

#include "Config.hpp"
#include "LAL.hpp"
#include "CString.hpp"
#include "OSPlatform.hpp"




namespace Renderer
{
	// Enums

	enum ERenderer
	{
		ERenderer_Width                 = 80,

	#ifdef Debug
		ERenderer_Height                = 48,

		ERenderer_BorderLine            = 24,

		ERenderer_DebugStart            = 25,
		ERenderer_DebugLogSize          = 18,

		ERenderer_DebugPersistentStart  = 44,
		ERenderer_PersistentSectionSize = 4 ,
	#else
		ERenderer_Height                = 24,
	#endif

		ERenderer_GameEnd               = 23,	
	};



	// Aliases (Typedefs)

	using CSBI = CONSOLE_SCREEN_BUFFER_INFO;
	using Cell = CHAR_INFO;
	using Line = Cell[ERenderer_Width];



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



	// Constants

	#define SizeOf_Renderer \
		sizeof(Data)



	// Functions

	void Clear(void);

	bool FillCellsWithWhitespace(void);

	bool FormatCells(void);

	const Data* GetContext(void);

	void LoadModule(void);

	void ProcessTiming(float64 _deltaTime);

	void RenderFrame(void);

	void ResetDrawPosition(void);

	void UnloadModule(void);

	void Update(void);

	void WriteToBufferCells(Cell* _cells, COORD _initalCell, COORD _finalCell);

	// BS Fix for now:
	typedef wchar_t WideChar;   // From C_String.h

	void WriteToLog(WideChar* _logString);

	void WriteToPersistentSection(sInt _row, WideChar* _lineformat, ...);

	void Logs_ScrollUp(void);

	void Logs_ScrollDown(void);
}

