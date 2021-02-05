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
		float32 X = 0;
		float32 Y = 0;
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

		bool AtFinish(Level& _level);

		bool IsGrounded(Level& _level);

		void Load();

		void Update(Level& _level);

		void Render();


		// Variables

		Cell Sprite = 
		{ 
			0, 
			BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE 
		};

		Vector2D Position = { -0.975f, 0.075f };

		float32 VerticalVelocity = 0.0f;

		bool 
			ShouldJump = false, 
			Fell       = false ;

		ECharacter_Move Active_MoveState = ECharacter_Move::Dont;
		ECharacter_Move MoveState        = ECharacter_Move::Dont;
	};

	// UI

	struct UI_Text
	{
		void Create 
		(
			WString _content, 
			COORD   _startingCell, 
			COORD   _endingCell,
			bool    _shouldCenter
		);

		void Render();


		WString Content;

		ptr<Cell> RenderCells = nullptr;

		COORD StartingCell { 0, 0 }, EndingCell { 0, 0 };
	};

	struct UI_Button
	{
		~UI_Button();

		void Create 
		(
			WString        _text, 
			COORD          _startingCell, 
			COORD          _endingCell, 
			bool           _shouldCenter,
			Void_Function& _callback
		);

		void Press ();
		void Render();


		UI_Text Text;

		ptr<Void_Function> Callback = nullptr;
	};

	struct UI_Grid
	{
		~UI_Grid();

		void Add 
		(
			WString        _text, 
			COORD          _startingCell, 
			COORD          _endingCell, 
			bool           _shouldCenter,
			Void_Function& _callback
		);

		void MoveUp  ();
		void MoveDown();
		void Select  ();
		void Render  ();


		ptr<UI_Button> Buttons = nullptr;

		uIntDM Num = 0;

		uIntDM CurrentIndex = 0;
	};

	struct UI_Widget
	{
		~UI_Widget();

		void AddText
		(
			WString _text,
			COORD   _startingCell,
			COORD   _endingCell,
			bool    _shouldCenter
		);

		void AddButton 
		(
			WString        _text,
			COORD          _startingCell,
			COORD          _endingCell,
			bool           _shouldCenter,
			Void_Function& _callback
		);

		void MoveUp  ();
		void MoveDown();
		void Select  ();
		void Render  ();


		ptr<UI_Text> TextUIs = nullptr;

		uIntDM Num_TextUIs = 0;

		UI_Grid Grid;
	};



	// Functions

	// Space

	COORD Convert_Vector2D_ToRenderCoord(Vector2D _vector);

	// General Rendering

	void ChangeCellsTo_Grey(ptr<Cell> _renderCells, uIntDM _length);

	void ChangeCellsTo_White(ptr<Cell> _renderCells, uIntDM _length);
}


