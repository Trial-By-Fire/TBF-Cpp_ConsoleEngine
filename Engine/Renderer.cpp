// Parent Header
#include "Renderer.hpp"



// Includes

#include "FloatCompare.hpp"
#include "Input.hpp"
#include "Memory.hpp"
#include "Timing.hpp"
#include "State.hpp"



using OSPlatform::Console_Cursor_MinSize;
using OSPlatform::Console_Cursor_NotVisible;
using OSPlatform::Console_WhiteCell;

using Cell       = Renderer::Cell;
using Data       = Renderer::Data;
using ScreenInfo = Renderer::ScreenInfo;
using Line       = Renderer::Line;

CompileTime auto PersistentSize = Renderer::PersistentSize;


// Static Data

// Private

ptr<Cell> Buffer;

Data       Renderer::Context;
ScreenInfo Screen;

Line Border_GameDebug    ;
Line Border_LogPersistent;

// Eliminate these?

struct Lines
{
	ptr<Line> Array;

	size_t Num;
};
	
Lines DebugLogSection_Dynamic;

uInt DebugLogSection_RelativeLastLine = 1;


#ifdef BuildDebug 

	Line PersistentSection[PersistentSize];

#endif

ro CTS_CWString Renderer_ConsoleTitle = L"TBF C++ Engine";

ro COORD Console_ScreenPos_00 = 
{
	0,  // X
	0   // Y
};



// Functions

// Public

void Renderer::Clear(void)
{
	if (UpdateConsoleInfo())
	{
		Memory::FormatByFill(Buffer, 0, BufferWidth * BufferHeight);
	}
}

// Return value tells you if it did the job.
bool Renderer::FillCellsWithWhitespace(void)
{
	return FillConsoleOutputCharacter
	(
		Context.Output_Handle,
		(TCHAR)' ',
		Context.BufferSize,
		Console_ScreenPos_00,
		&Context.CharactersWritten
	);
}

bool Renderer::FormatCells(void)
{
	return FillConsoleOutputAttribute
	(
		Context.Output_Handle,
		Context.CSBI_Instance.wAttributes,
		Context.BufferSize,
		Console_ScreenPos_00,
		&Context.CharactersWritten
	);
}

ro Data& Renderer::GetContext(void)
{
	return Context;
}

void Renderer::LoadModule(void)
{
	SetupConsole();

	InitalizeData();

	// Setup Console to ideal configuration.

	SetConsoleTitle(Renderer_ConsoleTitle);

	UpdateSizeAndPosition();

	return;
}

void Renderer::ProcessTiming()
{
	Context.RefreshTimer.Tick();
}

void Renderer::RenderFrame(void)
{
	// Renders buffer to console.
	WriteConsoleOutput(Context.Output_Handle, Buffer, Context.CoordSize, Console_ScreenPos_00, &Context.Size);

	return;
}

void Renderer::ResetDrawPosition(void)
{
	SetConsoleCursorPosition(Context.Output_Handle, Console_ScreenPos_00);

	return;
}

void Renderer::UnloadModule(void)
{
	if (OSPlatform::Unbind_IOBufferTo_Console() != true)
	{
		perror("Failed to unbind standard IO from renderer console");

		exit(1);
	}

	if (static_cast<bool>(FreeConsole()) != true)
	{
		perror("Failed to free renderer console properly.");

		exit(1);
	}
}

void Renderer::Update(void)
{
	if (Context.RefreshTimer.Ended())
	{
		Clear();

		DrawGameScanlines();

		State::GetEngineState()->Render();

		//WriteToPersistentSection(1, L"Relative Last Line: %u", DebugLogSection_RelativeLastLine);

		if CompileTime (DebugEnabled)
		{
			COORD 
				startingCell = { 0          , BorderLineRow }, 
				finalCell    = { BufferWidth, BorderLineRow };

			WriteToBufferCells(RCast<Cell>(getPtr(Border_GameDebug)), startingCell, finalCell);

			startingCell.Y = PersistentStart - 1;
			finalCell   .Y = PersistentStart - 1;

			WriteToBufferCells(RCast<Cell>(getPtr(Border_LogPersistent)), startingCell, finalCell);

			if (DebugLogSection_Dynamic.Num <= 18)
			{
				for (uIntDM index = 0; index < DebugLogSection_Dynamic.Num - 1; index++)
				{
					startingCell.Y = DebugStart + static_cast<uInt16>(index);
					finalCell   .Y = DebugStart + static_cast<uInt16>(index);

					WriteToBufferCells(DebugLogSection_Dynamic.Array[index], startingCell, finalCell);
				}
			}
			else
			{
				uIntDM LogStart = DebugLogSection_Dynamic.Num - 18 - DebugLogSection_RelativeLastLine;

				for (uIntDM index = 0; index < LogSize; index++)
				{
					startingCell.Y = DebugStart + static_cast<uInt16>(index);
					finalCell   .Y = DebugStart + static_cast<uInt16>(index);

					WriteToBufferCells(DebugLogSection_Dynamic.Array[LogStart + index], startingCell, finalCell);
				}
			}

			for (uIntDM index = 0; index < PersistentSize; index++)
			{
				startingCell.Y = PersistentStart + static_cast<uInt16>(index);
				finalCell   .Y = PersistentStart + static_cast<uInt16>(index);

				WriteToBufferCells((Cell*)&PersistentSection[index], startingCell, finalCell);
			}
		}

		RenderFrame();

		Context.RefreshTimer.Reset();
	}
}

void Renderer::WriteToBufferCells(Cell* _cell, COORD _initalCell, COORD _finalCell)
{
	uIntDM lineOffset = _initalCell.Y * BufferWidth;
	uIntDM colOffset  = _initalCell.X;

	uIntDM totalOffset = lineOffset + colOffset;

	ptr<Cell> bufferOffset = getPtr(Buffer[totalOffset]);

	uIntDM dataSize = totalOffset;

	lineOffset =  _finalCell.Y * BufferWidth;
	colOffset  =  _finalCell.X              ;

	totalOffset = lineOffset + colOffset;

	dataSize = totalOffset - dataSize;

	if (dataSize == 0) dataSize = 1;

	Memory::FormatWithData(bufferOffset, _cell, dataSize);

	return;
}

void Renderer_DebugLogDynamic_AddLine(void)
{
	if CompileTime (DebugEnabled)
	{
		if (DebugLogSection_Dynamic.Num == 0)
		{
			DebugLogSection_Dynamic.Array = Memory::GlobalAllocate<Line>(1);

			DebugLogSection_Dynamic.Num++;
		}
		else
		{
			ptr<Line> resizeIntermediary = Memory::GlobalReallocate<Line>(DebugLogSection_Dynamic.Array, (DebugLogSection_Dynamic.Num + 1));

			if (resizeIntermediary != nullptr)
			{
				DebugLogSection_Dynamic.Array = resizeIntermediary;

				DebugLogSection_Dynamic.Num++;
			}
			else
			{
				perror("Failed to globally reallocate log line array.");

				exit(1);
			}
		}
	}
}

void Renderer::WriteToLog(ptr<ro WideChar> _logString)
{
	if CompileTime (DebugEnabled)
	{
		unbound uInt nextLine = 0;

		uIntDM logLength = wcslen(_logString);
		uIntDM linePos = 0;

		if (nextLine == 0)
		{
			Renderer_DebugLogDynamic_AddLine();
		}

		for (uIntDM index = 0; index < logLength; index++)
		{
			if (linePos > BufferWidth - 1)
			{
				nextLine++;

				Renderer_DebugLogDynamic_AddLine();

				linePos = 0;
			}

			DebugLogSection_Dynamic.Array[nextLine][linePos].Char.UnicodeChar = _logString[index];
			DebugLogSection_Dynamic.Array[nextLine][linePos].Attributes       = Console_WhiteCell;

			linePos++;
		}

		for (uIntDM index = linePos; index < BufferWidth; index++)
		{
			DebugLogSection_Dynamic.Array[nextLine][index].Char.UnicodeChar = 0;
			DebugLogSection_Dynamic.Array[nextLine][index].Attributes       = 0;
		}

		nextLine++;

		Renderer_DebugLogDynamic_AddLine();

		DebugLogSection_RelativeLastLine = 1;
	}
}

// Note: Row starts at 1.
void Renderer::WriteToPersistentSection(sInt _row, ptr<ro WideChar> _lineformat, ...)
{
	if CompileTime (DebugEnabled)
	{
		WideChar TranslationBuffer[BufferWidth];

		Cell* PersistentSubSection = PersistentSection[_row - 1];

		sInt CellsFormatted;

		va_list argList;


		va_start(argList, _lineformat);

		CellsFormatted =

			// Windows hard coding.
			_vswprintf_s_l
			(
			TranslationBuffer,
			BufferWidth,
			_lineformat,
			nullptr,
			argList
			);

		va_end(argList);

		for (uIntDM index = 0; index < CellsFormatted; index++)
		{
			PersistentSubSection[index].Char.UnicodeChar = TranslationBuffer[index];
			PersistentSubSection[index].Attributes       = Console_WhiteCell;
		}

		for (uIntDM index = CellsFormatted + 1; index < BufferWidth; index++)
		{
			PersistentSubSection[index].Char.UnicodeChar = NULL;
			PersistentSubSection[index].Attributes       = NULL;
		}
	}
}



// Private

void Renderer::DrawGameScanlines(void)
{
	unbound COORD cellIndex = { 0, 0 };

	ro WideChar blockChar = '-';

	Cell cellUnit;

	cellUnit.Char.UnicodeChar = blockChar;
	cellUnit.Attributes       = FOREGROUND_INTENSITY;

	Cell cellLine[BufferWidth];

	for (uIntDM index = 0; index < BufferWidth; index++)
	{
		cellLine[index] = cellUnit;
	}

	COORD cellIndex_End = { BufferWidth, cellIndex.Y };

	WriteToBufferCells(RCast<Cell>(getPtr(cellLine)), cellIndex, cellIndex_End);

	cellIndex.Y++;

	if (cellIndex.X >= BufferWidth)
	{
		cellIndex.X = 0;

		cellIndex.Y++;
	}

	if (cellIndex.Y > GameEnd)
	{
		cellIndex.X = 0;

		cellIndex.Y = 0;
	}
}

void Renderer::InitalizeData(void)
{
	// Setup Necessary Data

	Screen.Center.X = GetSystemMetrics(SM_CXSCREEN) / 2;
	Screen.Center.Y = GetSystemMetrics(SM_CYSCREEN) / 2;

	Context.ScreenPosition.X = (Screen.Center.X - ((BufferWidth  / 2) * 8)) - 20;
	Context.ScreenPosition.Y = (Screen.Center.Y - ((BufferHeight / 2) * 8)) - 200;

	Context.Output_Handle = GetStdHandle(OSPlatform::StdHandle::Output);

	Context.Window_Handle = GetConsoleWindow();

	Context.Size.Left   = Console_ScreenPos_00.X;
	Context.Size.Top    = Console_ScreenPos_00.Y;
	Context.Size.Right  = BufferWidth  - 1;
	Context.Size.Bottom = BufferHeight - 1;

	Context.CursorSettings.dwSize   = Console_Cursor_MinSize;
	Context.CursorSettings.bVisible = Console_Cursor_NotVisible;

	Buffer = Memory::GlobalAllocate<Cell>(BufferWidth * BufferHeight);

	Memory::FormatByFill(Buffer, 0, BufferWidth * BufferHeight);

	Cell borderCell; 
	
	borderCell.Char.UnicodeChar = '='; 
	borderCell.Attributes       = Console_WhiteCell;

	if CompileTime (DebugEnabled)
	{
		for (size_t index = 0; index < BufferWidth; index++)
		{
			Border_GameDebug    [index] = borderCell;
			Border_LogPersistent[index] = borderCell;
		}

		DebugLogSection_Dynamic.Array = nullptr;
		DebugLogSection_Dynamic.Num   = 0;
	}

	return;
}

// Do initial request and IO binding for console interface.
void Renderer::SetupConsole(void)
{
	if (OSPlatform::RequestConsole() != true)
	{
		perror("Failed to get console for rendering from operating system.");

		exit(1);
	}

	if (OSPlatform::Bind_IOBufferTo_Console() != true)
	{
		perror("Failed to bind standard IO to renderer console.");

		exit(1);
	}

	return;
}

bool Renderer::UpdateConsoleInfo(void)
{
	return GetConsoleScreenBufferInfo(Context.Output_Handle, &Context.CSBI_Instance);
}

void Renderer::UpdateSizeAndPosition(void)
{
	// Set desired cursor preferences.
	SetConsoleCursorInfo(Context.Output_Handle, &Context.CursorSettings);

	// Change the window size.
	SetConsoleWindowInfo(Context.Output_Handle, true, &Context.Size);

	// Update the buffer size.
	SetConsoleScreenBufferSize(Context.Output_Handle, Context.CoordSize);

	// Update the window size.
	SetConsoleWindowInfo(Context.Output_Handle, true, &Context.Size);

	SetWindowPos
	(
		Context.Window_Handle,
		HWND_TOP,
		Context.ScreenPosition.X,
		Context.ScreenPosition.Y,
		0,
		0,
		SWP_NOSIZE
	);

	return;
}

void Renderer::Logs_ScrollUp(void)
{
	if CompileTime (DebugEnabled)
	{
		if (DebugLogSection_RelativeLastLine < DebugLogSection_Dynamic.Num)
		{
			DebugLogSection_RelativeLastLine++;
		}
	}
}

void Renderer::Logs_ScrollDown(void)
{
	if CompileTime (DebugEnabled)
	{
		if (DebugLogSection_RelativeLastLine > 1)
		{
			DebugLogSection_RelativeLastLine --;
		}
	}
}
