// Parent Header
#include "Renderer.hpp"



// Includes

#include "FloatCompare.hpp"
#include "Input.hpp"
#include "Memory.hpp"
#include "Timing.hpp"
#include "State.hpp"



// Static Data

// Private

CHAR_INFO* Buffer;

RendererData Renderer;
ScreenInfo   Screen;

Line Border_GameDebug    ;
Line Border_LogPersistent;

// Eliminate these?

typedef 
		
struct Lines_Def
{
	Line* Array;

	size_t Num;
}

Lines;
	
Lines DebugLogSection_Dynamic;

uInt DebugLogSection_RelativeLastLine = 1;


#ifdef Debug
	Line PersistentSection[ERenderer_PersistentSectionSize];
#endif

const CTS_CWString Renderer_ConsoleTitle = L"TBF C Engine: Phase 14";

const COORD Console_ScreenPos_00 = 
{
	0,  // X
	0   // Y
};



// Forward Declarations

void DrawGameScanlines    (void);
void InitalizeData        (void);
void SetupConsole         (void);
bool ShouldRender         (void);
bool UpdateConsoleInfo    (void);
void UpdateSizeAndPosition(void);



// Functions

// Public

void Renderer_Clear(void)
{
	if (UpdateConsoleInfo())
	{
		Memory_FormatByFill(CHAR_INFO, Buffer, 0, ERenderer_Width * ERenderer_Height);
	}
}

// Return value tells you if it did the job.
bool Renderer_FillCellsWithWhitespace(void)
{
	return FillConsoleOutputCharacter
	(
		Renderer.Output_Handle,
		(TCHAR)' ',
		Renderer.BufferSize,
		Console_ScreenPos_00,
		&Renderer.CharactersWritten
	);
}

bool Renderer_FormatCells(void)
{
	return FillConsoleOutputAttribute
	(
		Renderer.Output_Handle,
		Renderer.CSBI_Instance.wAttributes,
		Renderer.BufferSize,
		Console_ScreenPos_00,
		&Renderer.CharactersWritten
	);
}

const RendererData* Renderer_GetContext(void)
{
	return &Renderer;
}

void Renderer_LoadModule(void)
{
	SetupConsole();

	InitalizeData();

	// Setup Console to ideal configuration.

	SetConsoleTitle(Renderer_ConsoleTitle);

	UpdateSizeAndPosition();

	return;
}

void Renderer_ProcessTiming(float64 _deltaTime)
{
	if (Float64_ApproxEqual(_deltaTime, 0.000001L) || Float64_ApproxLess(_deltaTime, 0.000001L))
	{
		Renderer.RefeshTimer = Renderer.RefeshTimer + 0.000001L;

		return;
	}
	else
	{
		Renderer.RefeshTimer = Renderer.RefeshTimer + _deltaTime;

		return;
	}
}

void Renderer_RenderFrame(void)
{
	// Renders buffer to console.
	WriteConsoleOutput(Renderer.Output_Handle, Buffer, Renderer.CoordSize, Console_ScreenPos_00, &Renderer.Size);

	return;
}

void Renderer_ResetDrawPosition(void)
{
	SetConsoleCursorPosition(Renderer.Output_Handle, Console_ScreenPos_00);

	return;
}

void Renderer_UnloadModule(void)
{
	if (Unbind_IOBufferTo_Console() != true)
	{
		perror("Failed to unbind standard IO from renderer console");

		exit(1);
	}

	if (FreeConsole() != true)
	{
		perror("Failed to free renderer console properly.");

		exit(1);
	}
}

void Renderer_Update(void)
{
	if (ShouldRender())
	{
		Renderer_Clear();

		DrawGameScanlines();

		State_Render();

		//Renderer_WriteToPersistentSection(1, L"Relative Last Line: %u", DebugLogSection_RelativeLastLine);

	#ifdef Debug


		COORD 
			startingCell = { 0              , ERenderer_BorderLine}, 
			finalCell    = { ERenderer_Width, ERenderer_BorderLine};

		Renderer_WriteToBufferCells((Cell*)&Border_GameDebug, startingCell, finalCell);

		startingCell.Y = ERenderer_DebugPersistentStart - 1;
		finalCell   .Y = ERenderer_DebugPersistentStart - 1;

		Renderer_WriteToBufferCells((Cell*)&Border_LogPersistent, startingCell, finalCell);

		if (DebugLogSection_Dynamic.Num <= 18)
		{
			for (size_t index = 0; index < DebugLogSection_Dynamic.Num - 1; index++)
			{
				startingCell.Y = ERenderer_DebugStart + index;
				finalCell   .Y = ERenderer_DebugStart + index;

				Renderer_WriteToBufferCells(DebugLogSection_Dynamic.Array[index], startingCell, finalCell);
			}
		}
		else
		{
			size_t LogStart = DebugLogSection_Dynamic.Num - 18 - DebugLogSection_RelativeLastLine;

			for (size_t index = 0; index < ERenderer_DebugLogSize; index++)
			{
				startingCell.Y = ERenderer_DebugStart + index;
				finalCell   .Y = ERenderer_DebugStart + index;

				Renderer_WriteToBufferCells(DebugLogSection_Dynamic.Array[LogStart + index], startingCell, finalCell);
			}
		}

		for (size_t index = 0; index < ERenderer_PersistentSectionSize; index++)
		{
			startingCell.Y = ERenderer_DebugPersistentStart + index;
			finalCell   .Y = ERenderer_DebugPersistentStart + index;

			Renderer_WriteToBufferCells((Cell*)&PersistentSection[index], startingCell, finalCell);
		}

	#endif

		Renderer_RenderFrame();

		Renderer.RefeshTimer = 0.0L;
	}
}

void Renderer_WriteToBufferCells(Cell* _cell, COORD _initalCell, COORD _finalCell)
{
	size_t lineOffset = _initalCell.Y * ERenderer_Width;
	size_t colOffset  = _initalCell.X;

	size_t totalOffset = lineOffset + colOffset;

	void* bufferOffset = &Buffer[totalOffset];

	size_t dataSize = totalOffset;

	lineOffset =  _finalCell.Y * ERenderer_Width;
	colOffset  =  _finalCell.X                  ;

	totalOffset = lineOffset + colOffset;

	dataSize = totalOffset - dataSize;

	if (dataSize == 0) dataSize = 1;

	Memory_FormatWithData(bufferOffset, _cell, dataSize * sizeof(Cell));

	return;
}

void Renderer_DebugLogDynamic_AddLine(void)
{
#ifdef Debug

	if (DebugLogSection_Dynamic.Num == 0)
	{
		DebugLogSection_Dynamic.Array = (Line*)GlobalAllocate(Line, 1);

		DebugLogSection_Dynamic.Num++;
	}
	else
	{
		Address resizeIntermediary = GlobalReallocate(Line, DebugLogSection_Dynamic.Array, (DebugLogSection_Dynamic.Num + 1));

		if (resizeIntermediary != NULL)
		{
			DebugLogSection_Dynamic.Array = (Line*)resizeIntermediary;

			DebugLogSection_Dynamic.Num++;
		}
		else
		{
			perror("Failed to globally reallocate log line array.");

			exit(1);
		}
	}

#endif
}

void Renderer_WriteToLog(WideChar* _logString)
{
#ifdef Debug


	static uInt nextLine = 0;

	size_t logLength = wcslen(_logString);
	size_t linePos   = 0;

	if (nextLine == 0)
	{
		Renderer_DebugLogDynamic_AddLine();
	}

	for (size_t index = 0; index < logLength; index++)
	{
		if (linePos > ERenderer_Width - 1)
		{
			nextLine++;

			Renderer_DebugLogDynamic_AddLine();

			linePos = 0;
		}

		DebugLogSection_Dynamic.Array[nextLine][linePos].Char.UnicodeChar = _logString[index];
		DebugLogSection_Dynamic.Array[nextLine][linePos].Attributes       = Console_WhiteCell;

		linePos++;
	}

	for (size_t index = linePos; index < ERenderer_Width; index++)
	{
		DebugLogSection_Dynamic.Array[nextLine][index].Char.UnicodeChar = 0;
		DebugLogSection_Dynamic.Array[nextLine][index].Attributes       = 0;
	}

	nextLine++;

	Renderer_DebugLogDynamic_AddLine();

	DebugLogSection_RelativeLastLine = 1;

#endif
}

// Note: Row starts at 1.
void Renderer_WriteToPersistentSection(sInt _row, WideChar* _lineformat, ...)
{
#ifdef Debug

	WideChar TranslationBuffer[ERenderer_Width];

	Cell* PersistentSubSection = PersistentSection[_row - 1];

	sInt CellsFormatted;

	va_list argList;


	va_start(argList, _lineformat);

	CellsFormatted = 
		
		// Windows hard coding.
		_vswprintf_s_l
		(
			TranslationBuffer, 
			ERenderer_Width, 
			_lineformat, 
			NULL,
			argList
		);

	va_end(argList);

	for (size_t index = 0; index < CellsFormatted; index++)
	{
		PersistentSubSection[index].Char.UnicodeChar = TranslationBuffer[index];
		PersistentSubSection[index].Attributes       = Console_WhiteCell;
	}

	for (size_t index = CellsFormatted + 1; index < ERenderer_Width; index++)
	{
		PersistentSubSection[index].Char.UnicodeChar = NULL;
		PersistentSubSection[index].Attributes       = NULL;
	}

#endif
}



// Private

void DrawGameScanlines(void)
{
	static COORD cellIndex = { 0, 0 };

	const WideChar blockChar = '-';

	Cell cellUnit;

	cellUnit.Char.UnicodeChar = blockChar;
	cellUnit.Attributes       = FOREGROUND_INTENSITY;

	Cell cellLine[ERenderer_Width];

	for (size_t index = 0; index < ERenderer_Width; index++)
	{
		cellLine[index] = cellUnit;
	}

	COORD cellIndex_End = { ERenderer_Width, cellIndex.Y };

	Renderer_WriteToBufferCells((Cell*)&cellLine, cellIndex, cellIndex_End);

	cellIndex.Y++;

	if (cellIndex.X >= ERenderer_Width)
	{
		cellIndex.X = 0;

		cellIndex.Y++;
	}

	if (cellIndex.Y > ERenderer_GameEnd)
	{
		cellIndex.X = 0;

		cellIndex.Y = 0;
	}
}

void InitalizeData(void)
{
	// Setup Necessary Data

	Screen.Center.X = GetSystemMetrics(SM_CXSCREEN) / 2;
	Screen.Center.Y = GetSystemMetrics(SM_CYSCREEN) / 2;

	Renderer.ScreenPosition.X = (Screen.Center.X - ((ERenderer_Width  / 2) * 8)) - 20;
	Renderer.ScreenPosition.Y = (Screen.Center.Y - ((ERenderer_Height / 2) * 8)) - 200;

	Renderer.RefeshTimer    = 0.0L;
	Renderer.RefeshInterval = 1.0L / 60.0L;

	Renderer.CoordSize.X = ERenderer_Width ;
	Renderer.CoordSize.Y = ERenderer_Height;

	Renderer.Output_Handle = GetStdHandle(STD_OUTPUT_HANDLE);

	Renderer.Window_Handle = GetConsoleWindow();

	Renderer.Size.Left   = Console_ScreenPos_00.X;
	Renderer.Size.Top    = Console_ScreenPos_00.Y;
	Renderer.Size.Right  = ERenderer_Width  - 1;
	Renderer.Size.Bottom = ERenderer_Height - 1;

	Renderer.CursorSettings.dwSize   = Console_Cursor_MinSize;
	Renderer.CursorSettings.bVisible = Console_Cursor_NotVisible;

	Buffer = (CHAR_INFO*)GlobalAllocate(CHAR_INFO, ERenderer_Width * ERenderer_Height);

	Memory_FormatByFill(CHAR_INFO, Buffer, 0, ERenderer_Width * ERenderer_Height);

	Cell borderCell; 
	
	borderCell.Char.UnicodeChar = '='; 
	borderCell.Attributes       = Console_WhiteCell;

#ifdef Debug

	for (size_t index = 0; index < ERenderer_Width; index++)
	{
		Border_GameDebug    [index] = borderCell;
		Border_LogPersistent[index] = borderCell;
	}

	DebugLogSection_Dynamic.Array = NULL;
	DebugLogSection_Dynamic.Num = 0;

#endif

	return;
}

// Do initial request and IO binding for console interface.
void SetupConsole(void)
{
	if (RequestConsole() != true)
	{
		perror("Failed to get console for rendering from operating system.");

		exit(1);
	}

	if (Bind_IOBufferTo_Console() != true)
	{
		perror("Failed to bind standard IO to renderer console.");

		exit(1);
	}

	return;
}

bool ShouldRender(void)
{
	return Float64_ApproxGreater(Renderer.RefeshTimer, Renderer.RefeshInterval) ||
		   Float64_ApproxEqual  (Renderer.RefeshTimer, Renderer.RefeshInterval)   ;
}

bool UpdateConsoleInfo(void)
{
	return GetConsoleScreenBufferInfo(Renderer.Output_Handle, &Renderer.CSBI_Instance);
}

void UpdateSizeAndPosition(void)
{
	// Set desired cursor preferences.
	SetConsoleCursorInfo(Renderer.Output_Handle, &Renderer.CursorSettings);

	// Change the window size.
	SetConsoleWindowInfo(Renderer.Output_Handle, true, &Renderer.Size);

	// Update the buffer size.
	SetConsoleScreenBufferSize(Renderer.Output_Handle, Renderer.CoordSize);

	// Update the window size.
	SetConsoleWindowInfo(Renderer.Output_Handle, true, &Renderer.Size);

	SetWindowPos
	(
		Renderer.Window_Handle,
		HWND_TOP,
		Renderer.ScreenPosition.X,
		Renderer.ScreenPosition.Y,
		0,
		0,
		SWP_NOSIZE
	);

	return;
}

void Renderer_Logs_ScrollUp(void)
{
#ifdef Debug

	if (DebugLogSection_RelativeLastLine < DebugLogSection_Dynamic.Num)
	{
		DebugLogSection_RelativeLastLine++;
	}

#endif
}

void Renderer_Logs_ScrollDown(void)
{
#ifdef Debug

	if (DebugLogSection_RelativeLastLine > 1)
	{
		DebugLogSection_RelativeLastLine --;
	}

#endif
}