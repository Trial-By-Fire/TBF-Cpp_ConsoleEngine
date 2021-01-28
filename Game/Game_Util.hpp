#pragma once



// Includes

#include "Engine/Engine.hpp"



namespace Game
{
	using OSPlatform::EKeyCode;
	using OSPlatform::Console_WhiteCell;

	using Renderer::Cell;
	using Renderer::Line;
	using Renderer::WriteToLog;
	using Renderer::WriteToBufferCells;
	using Renderer::WriteToPersistentSection;



	// Enums

	enum class ECharacter_Move
	{
		Dont ,
		Left ,
		Right
	};

	enum ELevelCell
	{
		Empty  = 0,
		Pit    = BACKGROUND_BLUE,
		Ground = BACKGROUND_GREEN,
		Finish = BACKGROUND_RED
	};



	// Aliases

	// Level

	using GameScreenBuffer = Line[Renderer::GameEnd + 1];

	using Level = GameScreenBuffer;



	// Structs

	// Space

	struct Vector2D
	{
		float32 X;
		float32 Y;
	};

	// Character

	struct Character
	{
		Cell Sprite;

		Vector2D Position;

		float32 VerticalVelocity;

		bool ShouldJump, Fell;

		ECharacter_Move Active_MoveState;
		ECharacter_Move MoveState;
	};

	// UI

	struct UI_Text
	{
		WideChar* Content;

		size_t Length;

		Cell* RenderCells;

		COORD StartingCell, EndingCell;
	};

	struct UI_Button
	{
		UI_Text Text;

		Void_Function* Callback;
	};

	struct UI_Grid
	{
		UI_Button* Buttons;

		size_t Num;

		size_t CurrentIndex;
	};

	struct UI_Widget
	{
		UI_Text* TextUIs;

		size_t Num_TextUIs;

		//Ptr(UI_Grid) Grids;

		UI_Grid Grid;
	};



	// Functions

	// Character

	bool Character_AtFinish(Character* _character, Level* _level);

	bool Character_IsGrounded(Character* _character, Level* _level);

	void Character_Load(Character* _character);

	void Character_Update(Character* _character, Level* _level);

	void Character_Render(Character* _character);

	// Level

	sInt Level_GetCellAtPosition(Level* _level, Vector2D _position);

	void Level_SetCells(Level* _level, COORD _firstCell, COORD _lastCell, ELevelCell _cellType);

	void Level_Render(Level* _level);

	// Space

	COORD Convert_Vector2D_ToRenderCoord(Vector2D _vector);

	// General Rendering

	void ChangeCellsTo_Grey(Cell* _renderCells, size_t _length);

	void ChangeCellsTo_White(Cell* _renderCells, size_t _length);

	// UI

	void UI_Text_Create 
	(
		UI_Text*  _uiText, 
		WideChar* _content, 
		COORD     _startingCell, 
		COORD     _endingCell,
		bool      _shouldCenter
	);

	void UI_Text_Render(ro UI_Text* _uiText);

	void UI_Button_Create 
	(
		   UI_Button*     _button, 
		ro WideChar*      _text, 
		   COORD          _startingCell, 
		   COORD          _endingCell, 
		   bool           _shouldCenter,
		   Void_Function* _callback
	);

	void UI_Button_Press (ro UI_Button* _uiButton);
	void UI_Button_Render(ro UI_Button* _uiButton);

	void UI_Grid_Add 
	(
		   UI_Grid*       _uiGrid, 
		ro WideChar*      _text, 
		   COORD          _startingCell, 
		   COORD          _endingCell, 
		   bool           _shouldCenter,
		   Void_Function* _callback
	);

	void UI_Grid_MoveUp  (UI_Grid* _uiGrid);
	void UI_Grid_MoveDown(UI_Grid* _uiGrid);
	void UI_Grid_Select  (UI_Grid* _uiGrid);
	void UI_Grid_Render  (UI_Grid* _uiGrid);

	void UI_Widget_AddText
	(
		   UI_Widget* _uiWidget,
		ro WideChar*  _text,
		   COORD     _startingCell,
		   COORD     _endingCell,
		   bool      _shouldCenter
	);

	void UI_Widget_AddButton 
	(
		   UI_Widget*     _uiWidget,
		ro WideChar*      _text,
		   COORD          _startingCell,
		   COORD          _endingCell,
		   bool           _shouldCenter,
		   Void_Function* _callback
	);

	void UI_Widget_MoveUp  (UI_Widget* _uiWidget);
	void UI_Widget_MoveDown(UI_Widget* _uiWidget);
	void UI_Widget_Select  (UI_Widget* _uiWidget);
	void UI_Widget_Render  (UI_Widget* _uiWidget);
}


