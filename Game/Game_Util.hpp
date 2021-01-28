#pragma once



// Includes

#include "Engine/Engine.hpp"



namespace Game
{
	using OSPlatform::EKeyCode;
	using OSPlatform::Console_WhiteCell;

	using Cell = Renderer::Cell;
	using Line = Renderer::Line;

	CompileTime auto WriteToBufferCells       = Renderer::WriteToBufferCells;
	CompileTime auto WriteToLog               = Renderer::WriteToLog;
	CompileTime auto WriteToPersistentSection = Renderer::WriteToPersistentSection;



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

	//using Level = GameScreenBuffer;



	// Structs

	// Space

	struct Vector2D
	{
		float32 X;
		float32 Y;
	};

	// Level

	struct Level
	{
		// Functions

		sInt GetCellAtPosition(Vector2D _position);

		void SetCells(COORD _firstCell, COORD _lastCell, ELevelCell _cellType);

		void Render();


		// Variables

		GameScreenBuffer Content;
	};

	// Character

	struct Character
	{
		// Functions

		bool AtFinish(Level* _level);

		bool IsGrounded(Level* _level);

		void Load();

		void Update(Level* _level);

		void Render();


		// Variables

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
		void Create 
		(
			ro WideChar* _content, 
			   COORD     _startingCell, 
			   COORD     _endingCell,
			   bool      _shouldCenter
		);

		void Render();


		WideChar* Content;

		size_t Length;

		Cell* RenderCells;

		COORD StartingCell, EndingCell;
	};

	struct UI_Button
	{
		void Create 
		(
			ro WideChar*      _text, 
			   COORD          _startingCell, 
			   COORD          _endingCell, 
			   bool           _shouldCenter,
			   Void_Function* _callback
		);

		void Press ();
		void Render();


		UI_Text Text;

		Void_Function* Callback;
	};

	struct UI_Grid
	{
		void Add 
		(
			ro WideChar*      _text, 
			   COORD          _startingCell, 
			   COORD          _endingCell, 
			   bool           _shouldCenter,
			   Void_Function* _callback
		);

		void MoveUp  ();
		void MoveDown();
		void Select  ();
		void Render  ();


		UI_Button* Buttons;

		size_t Num;

		size_t CurrentIndex;
	};

	struct UI_Widget
	{
		void AddText
		(
			ro WideChar*  _text,
			   COORD     _startingCell,
			   COORD     _endingCell,
			   bool      _shouldCenter
		);

		void AddButton 
		(
			ro WideChar*      _text,
			   COORD          _startingCell,
			   COORD          _endingCell,
			   bool           _shouldCenter,
			   Void_Function* _callback
		);

		void MoveUp  ();
		void MoveDown();
		void Select  ();
		void Render  ();


		UI_Text* TextUIs;

		size_t Num_TextUIs;

		//Ptr(UI_Grid) Grids;

		UI_Grid Grid;
	};



	// Functions

	// Space

	COORD Convert_Vector2D_ToRenderCoord(Vector2D _vector);

	// General Rendering

	void ChangeCellsTo_Grey(Cell* _renderCells, uIntDM _length);

	void ChangeCellsTo_White(Cell* _renderCells, uIntDM _length);
}


