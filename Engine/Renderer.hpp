#pragma once



// Includes

#include "Config.hpp"
#include "LAL.hpp"
#include "CString.hpp"
#include "OSPlatform.hpp"



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

typedef CONSOLE_SCREEN_BUFFER_INFO CSBI;

typedef CHAR_INFO Cell;

typedef Cell Line[ERenderer_Width];

typedef struct RendererData_Def RendererData;
typedef struct ScreenInfo_Def   ScreenInfo;


typedef struct Vec2D_Int_Def Vec2D_Int;



// Structures

struct Vec2D_Int_Def { sInt X, Y; };

struct RendererData_Def
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

struct ScreenInfo_Def
{
	Vec2D_Int Center;
};



// Constants

#define SizeOf_Renderer \
	sizeof(RendererData)



// Functions

void Renderer_Clear(void);

bool Renderer_FillCellsWithWhitespace(void);

bool Renderer_FormatCells(void);

const RendererData* Renderer_GetContext(void);

void Renderer_LoadModule(void);

void Renderer_ProcessTiming(float64 _deltaTime);

void Renderer_RenderFrame(void);

void Renderer_ResetDrawPosition(void);

void Renderer_UnloadModule(void);

void Renderer_Update(void);

void Renderer_WriteToBufferCells(Cell* _cells, COORD _initalCell, COORD _finalCell);

// BS Fix for now:
typedef wchar_t WideChar;   // From C_String.h

void Renderer_WriteToLog(WideChar* _logString);

void Renderer_WriteToPersistentSection(sInt _row, WideChar* _lineformat, ...);

void Renderer_Logs_ScrollUp(void);

void Renderer_Logs_ScrollDown(void);



