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
		
	using CAttribute      = Renderer::CAttribute;
	using CAttributeField = OSPlatform::CAttributeField;

	enum class ELevelCell : WORD
	{

		Empty  = 0,
		Pit    = WORD(CAttribute::BG_Blue ),
		Ground = WORD(CAttribute::BG_Green),
		Finish = WORD(CAttribute::BG_Red  )
	};



	// Aliases

	// Level

	using GameScreenBuffer = StaticArray<Line, Renderer::GameEnd + 1>;



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

		Cell GetCellAtPosition(Vector2D _position);

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

		Cell Sprite = Cell
		(
			0, 
			CAttributeField
			(
				CAttribute::BG_Intensity, 
				CAttribute::BG_Red, 
				CAttribute::BG_Green, 
				CAttribute::BG_Blue
			)
		);

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

		void SetToGrey ();
		void SetToWhite();


		WString Content;

		DynamicArray<Cell> RenderCells;

		COORD StartingCell { 0, 0 }, EndingCell { 0, 0 };
	};

	struct UI_Button
	{
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


		DynamicArray<UI_Button> Buttons;

		uIntDM CurrentIndex = 0;
	};

	struct UI_Widget
	{
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


		DynamicArray<UI_Text> TextUIs;

		UI_Grid Grid;
	};



	// Functions

	// Space

	COORD Convert_Vector2D_ToRenderCoord(Vector2D _vector);

	// General Rendering

	void ChangeCellsTo_Grey(ptr<Cell> _renderCells, uIntDM _length);

	void ChangeCellsTo_White(ptr<Cell> _renderCells, uIntDM _length);
}


