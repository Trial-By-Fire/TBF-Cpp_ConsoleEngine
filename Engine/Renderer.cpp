// Parent Header
#include "Renderer.hpp"



// Includes

#include "Input.hpp"
#include "Memory.hpp"
#include "Timing.hpp"
#include "State.hpp"



using OSPlatform::Console_Cursor_MinSize;
using OSPlatform::Console_Cursor_NotVisible;
using OSPlatform::Console_WhiteCell;

using Data       = Renderer::Data;
using ScreenInfo = Renderer::ScreenInfo;
using Line       = Renderer::Line;



// Static Data

// Private

using Cell = OSPlatform::ConsoleCell;

DynamicArray<Cell> Buffer;

Data       Renderer::Context;
ScreenInfo Screen;

Line Border_GameDebug    ;
Line Border_LogPersistent;

DynamicArray<Line> DebugLogSection_Dynamic;

uInt DebugLogSection_RelativeLastLine = 1;

StaticArray<Line, Renderer::PersistentSize> PersistentSection;

ro WString Renderer_ConsoleTitle = L"TBF C++ Engine";

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
		unbound Cell value = { 0, 0 };

		Memory::FormatByFill<Cell>(Buffer.data(), value, BufferWidth * BufferHeight);
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
		getPtr(Context.CharactersWritten)
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
		getPtr(Context.CharactersWritten)
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

	SetConsoleTitle(Renderer_ConsoleTitle.data());

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
	WriteConsoleOutput(Context.Output_Handle, RCast<CHAR_INFO>(Buffer.data()), Context.CoordSize, Console_ScreenPos_00, getPtr(Context.Num));
}

void Renderer::ResetDrawPosition(void)
{
	SetConsoleCursorPosition(Context.Output_Handle, Console_ScreenPos_00);
}

void Renderer::UnloadModule(void)
{
	if (OSPlatform::Unbind_IOBufferTo_Console() != true)
	{
		throw RuntimeError("Failed to unbind standard IO from renderer console");
	}

	if (static_cast<bool>(FreeConsole()) != true)
	{
		throw RuntimeError("Failed to free renderer console properly.");
	}
}

void Renderer::Update(void)
{
	if (Context.RefreshTimer.Ended())
	{
		Clear();

		DrawGameScanlines();

		State::GetEngineState()->Render();

		if CompileTime (DebugEnabled == true)
		{
			COORD 
				startingCell = { 0          , BorderLineRow }, 
				finalCell    = { BufferWidth, BorderLineRow };

			WriteToBufferCells(Border_GameDebug.data(), startingCell, finalCell);

			startingCell.Y = PersistentStart - 1;
			finalCell   .Y = PersistentStart - 1;

			WriteToBufferCells(Border_LogPersistent.data(), startingCell, finalCell);

			startingCell.Y = DebugStart;
			finalCell   .Y = DebugStart;

			if (DebugLogSection_Dynamic.size() <= 18)
			{
				for (auto& line : DebugLogSection_Dynamic)
				{
					startingCell.Y++;
					finalCell   .Y++;

					WriteToBufferCells(line.data(), startingCell, finalCell);
				}
			}
			else
			{
				uIntDM LogStart = DebugLogSection_Dynamic.size() - LogSize - DebugLogSection_RelativeLastLine;

				for (uIntDM index = 0; index < LogSize; index++)
				{
					WriteToBufferCells(DebugLogSection_Dynamic[LogStart + index].data(), startingCell, finalCell);

					startingCell.Y++;
					finalCell   .Y++;
				}
			}

			startingCell.Y = PersistentStart;
			finalCell   .Y = PersistentStart;

			for (auto& line : PersistentSection)
			{
				WriteToBufferCells(line.data(), startingCell, finalCell);

				startingCell.Y++;
				finalCell   .Y++;
			}
		}

		RenderFrame();

		Context.RefreshTimer.Reset();
	}
}

void Renderer::WriteToBufferCells(ro ptr<Cell> _cell, COORD _initalCell, COORD _finalCell)
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

void Renderer::WriteToLog(WString _logString)
{
	if CompileTime (DebugEnabled == true)
	{
		unbound uInt nextLine = 0;

		uIntDM linePos = 0;

		if (nextLine == 0)
		{
			DebugLogSection_Dynamic.resize(1);
		}

		for (WChar wChar : _logString)
		{
			if (linePos > BufferWidth - 1)
			{
				nextLine++;

				DebugLogSection_Dynamic.resize(DebugLogSection_Dynamic.size() + 1);

				linePos = 0;
			}

			DebugLogSection_Dynamic[nextLine][linePos].Char       = wChar;
			DebugLogSection_Dynamic[nextLine][linePos].Attributes = Console_WhiteCell;

			linePos++;
		}

		for (uIntDM index = linePos; index < BufferWidth; index++)
		{
			DebugLogSection_Dynamic[nextLine][index].Char = 0;
			DebugLogSection_Dynamic[nextLine][index].Attributes.Reset();
		}

		nextLine++;

		DebugLogSection_Dynamic.resize(DebugLogSection_Dynamic.size() + 1);

		DebugLogSection_RelativeLastLine = 1;
	}
}

// Note: Row starts at 1.
void Renderer::WriteToPersistentSection(sInt _row, WString _lineformat, ...)
{
	if CompileTime (DebugEnabled == true)
	{
		ptr<Cell> PersistentSubSection = PersistentSection[_row - 1].data();

		for (uIntDM index = 0; index < _lineformat.size(); index++)
		{
			PersistentSubSection[index].Char       = _lineformat[index];
			PersistentSubSection[index].Attributes = Console_WhiteCell;
		}

		for (uIntDM index = _lineformat.size() + 1; index < BufferWidth; index++)
		{
			PersistentSubSection[index].Char = 0;
			PersistentSubSection[index].Attributes.Reset();
		}
	}
}



// Private

void Renderer::DrawGameScanlines(void)
{
	unbound COORD cellIndex = { 0, 0 };

	StaticArray<Cell, BufferWidth> cellLine;

	Cell cellUnit('-', CAttribute::FG_Intensity);

	Memory::FormatByFill(cellLine.data(), cellUnit, BufferWidth);

	COORD cellIndex_End = { BufferWidth, cellIndex.Y };

	WriteToBufferCells(cellLine.data(), cellIndex, cellIndex_End);

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

	Context.Num.Left   = Console_ScreenPos_00.X;
	Context.Num.Top    = Console_ScreenPos_00.Y;
	Context.Num.Right  = BufferWidth  - 1;
	Context.Num.Bottom = BufferHeight - 1;

	Context.CursorSettings.dwSize   = Console_Cursor_MinSize;
	Context.CursorSettings.bVisible = Console_Cursor_NotVisible;

	Buffer.resize(BufferWidth * BufferHeight);

	Cell borderCell = { '=', Console_WhiteCell }; 
	
	if CompileTime (DebugEnabled)
	{
		for (size_t index = 0; index < BufferWidth; index++)
		{
			Border_GameDebug    [index] = borderCell;
			Border_LogPersistent[index] = borderCell;
		}
	}

	return;
}

// Do initial request and IO binding for console interface.
void Renderer::SetupConsole(void)
{
	if (OSPlatform::RequestConsole() != true)
	{
		throw RuntimeError("Failed to get console for rendering from operating system.");
	}

	if (OSPlatform::Bind_IOBufferTo_Console() != true)
	{
		throw RuntimeError("Failed to bind standard IO to renderer console.");
	}

	return;
}

bool Renderer::UpdateConsoleInfo(void)
{
	return GetConsoleScreenBufferInfo(Context.Output_Handle, getPtr(Context.CSBI_Instance));
}

void Renderer::UpdateSizeAndPosition(void)
{
	// Set desired cursor preferences.
	SetConsoleCursorInfo(Context.Output_Handle, getPtr(Context.CursorSettings));

	// Change the window size.
	SetConsoleWindowInfo(Context.Output_Handle, true, getPtr(Context.Num));

	// Update the buffer size.
	SetConsoleScreenBufferSize(Context.Output_Handle, Context.CoordSize);

	// Update the window size.
	SetConsoleWindowInfo(Context.Output_Handle, true, getPtr(Context.Num));

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
	if CompileTime (DebugEnabled == true)
	{
		if (DebugLogSection_RelativeLastLine < (DebugLogSection_Dynamic.size() - LogSize))
		{
			DebugLogSection_RelativeLastLine++;
		}
	}
}

void Renderer::Logs_ScrollDown(void)
{
	if CompileTime (DebugEnabled == true)
	{
		if (DebugLogSection_RelativeLastLine > 1)
		{
			DebugLogSection_RelativeLastLine --;
		}
	}
}
