// Parent Header
#include "Renderer.hpp"



// Includes

#include "FloatCompare.hpp"
#include "Input.hpp"
#include "Memory.hpp"
#include "Timing.hpp"
#include "State.hpp"



namespace Renderer
{
	using OSPlatform::Console_Cursor_MinSize;
	using OSPlatform::Console_Cursor_NotVisible;
	using OSPlatform::Console_WhiteCell;


	// Static Data

	// Private

	Cell* Buffer;

	Data       Renderer;
	ScreenInfo Screen;

	Line Border_GameDebug    ;
	Line Border_LogPersistent;

	// Eliminate these?

	struct Lines
	{
		Line* Array;

		size_t Num;
	};
		
	Lines DebugLogSection_Dynamic;

	uInt DebugLogSection_RelativeLastLine = 1;


	#ifdef BuildDebug 

		Line PersistentSection[PersistentSize];

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

	void Clear(void)
	{
		if (UpdateConsoleInfo())
		{
			Memory_FormatByFill(Cell, Buffer, 0, BufferWidth * BufferHeight);
		}
	}

	// Return value tells you if it did the job.
	bool FillCellsWithWhitespace(void)
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

	bool FormatCells(void)
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

	ro Data* GetContext(void)
	{
		return &Renderer;
	}

	void LoadModule(void)
	{
		SetupConsole();

		InitalizeData();

		// Setup Console to ideal configuration.

		SetConsoleTitle(Renderer_ConsoleTitle);

		UpdateSizeAndPosition();

		return;
	}

	void ProcessTiming(float64 _deltaTime)
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

	void RenderFrame(void)
	{
		// Renders buffer to console.
		WriteConsoleOutput(Renderer.Output_Handle, Buffer, Renderer.CoordSize, Console_ScreenPos_00, &Renderer.Size);

		return;
	}

	void ResetDrawPosition(void)
	{
		SetConsoleCursorPosition(Renderer.Output_Handle, Console_ScreenPos_00);

		return;
	}

	void UnloadModule(void)
	{
		if (OSPlatform::Unbind_IOBufferTo_Console() != true)
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

	void Update(void)
	{
		if (ShouldRender())
		{
			Clear();

			DrawGameScanlines();

			State::Render();

			//WriteToPersistentSection(1, L"Relative Last Line: %u", DebugLogSection_RelativeLastLine);

			if CompileTime (DebugEnabled)
			{
				COORD 
					startingCell = { 0          , BorderLineRow }, 
					finalCell    = { BufferWidth, BorderLineRow };

				WriteToBufferCells((Cell*)&Border_GameDebug, startingCell, finalCell);

				startingCell.Y = PersistentStart - 1;
				finalCell   .Y = PersistentStart - 1;

				WriteToBufferCells((Cell*)&Border_LogPersistent, startingCell, finalCell);

				if (DebugLogSection_Dynamic.Num <= 18)
				{
					for (uIntDM index = 0; index < DebugLogSection_Dynamic.Num - 1; index++)
					{
						startingCell.Y = DebugStart + index;
						finalCell   .Y = DebugStart + index;

						WriteToBufferCells(DebugLogSection_Dynamic.Array[index], startingCell, finalCell);
					}
				}
				else
				{
					uIntDM LogStart = DebugLogSection_Dynamic.Num - 18 - DebugLogSection_RelativeLastLine;

					for (uIntDM index = 0; index < LogSize; index++)
					{
						startingCell.Y = DebugStart + index;
						finalCell   .Y = DebugStart + index;

						WriteToBufferCells(DebugLogSection_Dynamic.Array[LogStart + index], startingCell, finalCell);
					}
				}

				for (uIntDM index = 0; index < PersistentSize; index++)
				{
					startingCell.Y = PersistentStart + index;
					finalCell   .Y = PersistentStart + index;

					WriteToBufferCells((Cell*)&PersistentSection[index], startingCell, finalCell);
				}
			}

			RenderFrame();

			Renderer.RefeshTimer = 0.0L;
		}
	}

	void WriteToBufferCells(Cell* _cell, COORD _initalCell, COORD _finalCell)
	{
		uIntDM lineOffset = _initalCell.Y * BufferWidth;
		uIntDM colOffset  = _initalCell.X;

		uIntDM totalOffset = lineOffset + colOffset;

		void* bufferOffset = &Buffer[totalOffset];

		uIntDM dataSize = totalOffset;

		lineOffset =  _finalCell.Y * BufferWidth;
		colOffset  =  _finalCell.X                  ;

		totalOffset = lineOffset + colOffset;

		dataSize = totalOffset - dataSize;

		if (dataSize == 0) dataSize = 1;

		Memory::FormatWithData(bufferOffset, _cell, dataSize * sizeof(Cell));

		return;
	}

	void Renderer_DebugLogDynamic_AddLine(void)
	{
		if CompileTime (DebugEnabled)
		{
			if (DebugLogSection_Dynamic.Num == 0)
			{
				DebugLogSection_Dynamic.Array = (Line*)GlobalAllocate(Line, 1);

				DebugLogSection_Dynamic.Num++;
			}
			else
			{
				Memory::Address resizeIntermediary = GlobalReallocate(Line, DebugLogSection_Dynamic.Array, (DebugLogSection_Dynamic.Num + 1));

				if (resizeIntermediary != nullptr)
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
		}
	}

	void WriteToLog(WideChar* _logString)
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
				DebugLogSection_Dynamic.Array[nextLine][linePos].Attributes = Console_WhiteCell;

				linePos++;
			}

			for (uIntDM index = linePos; index < BufferWidth; index++)
			{
				DebugLogSection_Dynamic.Array[nextLine][index].Char.UnicodeChar = 0;
				DebugLogSection_Dynamic.Array[nextLine][index].Attributes = 0;
			}

			nextLine++;

			Renderer_DebugLogDynamic_AddLine();

			DebugLogSection_RelativeLastLine = 1;
		}
	}

	// Note: Row starts at 1.
	void WriteToPersistentSection(sInt _row, WideChar* _lineformat, ...)
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
				PersistentSubSection[index].Attributes = Console_WhiteCell;
			}

			for (uIntDM index = CellsFormatted + 1; index < BufferWidth; index++)
			{
				PersistentSubSection[index].Char.UnicodeChar = NULL;
				PersistentSubSection[index].Attributes = NULL;
			}
		}
	}



	// Private

	void DrawGameScanlines(void)
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

		WriteToBufferCells((Cell*)&cellLine, cellIndex, cellIndex_End);

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

	void InitalizeData(void)
	{
		// Setup Necessary Data

		Screen.Center.X = GetSystemMetrics(SM_CXSCREEN) / 2;
		Screen.Center.Y = GetSystemMetrics(SM_CYSCREEN) / 2;

		Renderer.ScreenPosition.X = (Screen.Center.X - ((BufferWidth  / 2) * 8)) - 20;
		Renderer.ScreenPosition.Y = (Screen.Center.Y - ((BufferHeight / 2) * 8)) - 200;

		Renderer.RefeshTimer    = 0.0L;
		Renderer.RefeshInterval = 1.0L / 60.0L;

		Renderer.CoordSize.X = BufferWidth ;
		Renderer.CoordSize.Y = BufferHeight;

		Renderer.Output_Handle = GetStdHandle(OSPlatform::StdHandle::Output);

		Renderer.Window_Handle = GetConsoleWindow();

		Renderer.Size.Left   = Console_ScreenPos_00.X;
		Renderer.Size.Top    = Console_ScreenPos_00.Y;
		Renderer.Size.Right  = BufferWidth  - 1;
		Renderer.Size.Bottom = BufferHeight - 1;

		Renderer.CursorSettings.dwSize   = Console_Cursor_MinSize;
		Renderer.CursorSettings.bVisible = Console_Cursor_NotVisible;

		Buffer = (Cell*)GlobalAllocate(Cell, BufferWidth * BufferHeight);

		Memory_FormatByFill(Cell, Buffer, 0, BufferWidth * BufferHeight);

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
	void SetupConsole(void)
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

	void Logs_ScrollUp(void)
	{
		if CompileTime (DebugEnabled)
		{
			if (DebugLogSection_RelativeLastLine < DebugLogSection_Dynamic.Num)
			{
				DebugLogSection_RelativeLastLine++;
			}
		}
	}

	void Logs_ScrollDown(void)
	{
		if CompileTime (DebugEnabled)
		{
			if (DebugLogSection_RelativeLastLine > 1)
			{
				DebugLogSection_RelativeLastLine --;
			}
		}
	}
}
