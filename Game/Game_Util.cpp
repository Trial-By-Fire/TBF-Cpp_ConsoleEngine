// Parent Header
#include "Game_Util.hpp"




namespace Game
{
	// Functions

	// Public

	// Character

	bool Character::AtFinish(Level& _level)
	{
		Vector2D collisionPostion = Position;

		collisionPostion.Y -= 0.085f;

		ELevelCell cellCollided = ELevelCell(WORD(_level.GetCellAtPosition(collisionPostion).Attributes));

		return cellCollided == ELevelCell::Finish;
	}

	bool Character::IsGrounded(Level& _level)
	{
		Vector2D collisionPostion = Position;

		collisionPostion.Y -= 0.085f;

		ELevelCell cellCollided = ELevelCell(WORD(_level.GetCellAtPosition(collisionPostion).Attributes));

		return cellCollided == ELevelCell::Ground || cellCollided == ELevelCell::Finish;
	}

	void Character::Load()
	{
		Sprite = Cell(0, CAttributeField(CAttribute::BG_Intensity, CAttribute::BG_Red, CAttribute::BG_Green, CAttribute::BG_Blue));

		Position = { -0.975f, 0.075f };

		VerticalVelocity = 0.0f;

		ShouldJump = false,
		Fell       = false;

		Active_MoveState = ECharacter_Move::Dont;
		MoveState        = ECharacter_Move::Dont;
	}

	void Character::Update(Level& _level)
	{
		if (Fell == true) 
			return;

		float32 deltaTime = SCast<const float32>(Timing::GetContext().DeltaTime.count());

		static float32 velocity = 1.0f;
		static float32 gravity  = 0.00004f;

		Vector2D collisionPostion = Position;

		collisionPostion.Y -= 0.085f;

		ELevelCell cellTypeCollided =  ELevelCell(WORD(_level.GetCellAtPosition(collisionPostion).Attributes));

		if (cellTypeCollided == ELevelCell::Ground || cellTypeCollided == ELevelCell::Finish)
		{
			VerticalVelocity = 0.0f;

			Position.Y = -0.9f;

			Active_MoveState = MoveState;
		}
		else if (cellTypeCollided == ELevelCell::Pit)
		{
			VerticalVelocity = 0.0f;

			Position.Y = -0.9f;

			Fell = true;
		}
		else
		{
			VerticalVelocity -= gravity * deltaTime;

			Position.Y += VerticalVelocity;
		}

		if (cellTypeCollided == ELevelCell::Finish) 
			return;

		if (ShouldJump && cellTypeCollided == ELevelCell::Ground)
		{
			WriteToLog(L"Giving character jump velocity");

			VerticalVelocity = 0.075f * deltaTime;

			Position.Y = -0.75f;

			ShouldJump = false;
		}

		switch (Active_MoveState)
		{
			case ECharacter_Move::Left:
			{
				if (Position.X > -1.0f)
				{
					Position.X -= velocity * deltaTime;
				}

				break;
			}
			case ECharacter_Move::Right:
			{
				if (Position.X < 1.0f)
				{
					Position.X += velocity * deltaTime;
				}

				break;
			}
		}
	}

	void Character::Render()
	{
		if (Fell)
			return;

		COORD renderCoord = Convert_Vector2D_ToRenderCoord(Position);

		WriteToPersistentSection(1, WString(L"Pos X: ") + ToWString(Position.X));
		WriteToPersistentSection(3, WString(L"Pos Y: ") + ToWString(Position.Y));

		WriteToPersistentSection(2, WString(L"RC  X: ") + ToWString(renderCoord.X));
		WriteToPersistentSection(4, WString(L"RC  Y: ") + ToWString(renderCoord.Y));

		if (renderCoord.Y < 0) 
			renderCoord.Y = 0;
		if (renderCoord.Y > Renderer::GameEnd)
			renderCoord.Y = Renderer::GameEnd;

		WriteToBufferCells(getPtr(Sprite), renderCoord, renderCoord);

		if constexpr(DebugEnabled)
		{
			static Cell colliderView(0, CAttributeField(CAttribute::BG_Intensity, CAttribute::BG_Red, CAttribute::BG_Green));

			COORD colliderViewCoord;

			Vector2D collisionPostion = Position;

			collisionPostion.Y -= 0.085f;

			colliderViewCoord = Convert_Vector2D_ToRenderCoord(collisionPostion);

			if (colliderViewCoord.Y < 0) 
				colliderViewCoord.Y = 0;
			if (colliderViewCoord.Y > Renderer::GameEnd) 
				colliderViewCoord.Y = Renderer::GameEnd;

			WriteToBufferCells(getPtr(colliderView), colliderViewCoord, colliderViewCoord);
		}
	}

	// Level

	Cell Level::GetCellAtPosition(Vector2D _position)
	{
		COORD renderCoord = Convert_Vector2D_ToRenderCoord(_position);

		return Content[renderCoord.Y][renderCoord.X];
	}

	void Level::SetCells(COORD _firstCell, COORD _lastCell, ELevelCell _cellType) 
	{
		uIntDM lineOffset = _firstCell.Y;
		uIntDM colOffset  = _firstCell.X;

		ptr<Cell> bufferOffset = getPtr(Content[lineOffset][colOffset]);

		uIntDM cellsSkipped = lineOffset * Renderer::BufferWidth + colOffset;

		lineOffset = _lastCell.Y;
		colOffset  = _lastCell.X;

		uIntDM totalOffset = lineOffset * Renderer::BufferWidth + colOffset;

		uIntDM dataSize = totalOffset - cellsSkipped;

		if (dataSize == 0) 
			dataSize = 1;

		Cell cellFormat(0, CAttributeField(WORD(_cellType)));

		Memory::FormatByFill(bufferOffset, cellFormat, dataSize);
	}

	void Level::Render()
	{
		COORD 
			screenStart = {                     0,                 0 }, 
			screenEnd   = { Renderer::BufferWidth, Renderer::GameEnd } ;

		WriteToBufferCells(RCast<Cell>(this), screenStart, screenEnd);
	}

	// Space

	COORD Convert_Vector2D_ToRenderCoord(Vector2D _vector)
	{
		static float32 
			offsetX = SCast<float32>(Renderer::BufferWidth) / 2.0f, 
			offsetY = SCast<float32>(Renderer::GameEnd    ) / 2.0f;

		float32 
			convertedX = _vector.X * (SCast<float32>(Renderer::BufferWidth) / 2.0f), 
			convertedY = _vector.Y * (SCast<float32>(Renderer::GameEnd    ) / 2.0f);

		COORD renderingCoord;

		renderingCoord.X = SCast<sInt16>(convertedX + offsetX   );	
		renderingCoord.Y = SCast<sInt16>(offsetY    - convertedY);

		if (renderingCoord.Y < 0) 
			renderingCoord.Y = 0;
		if (renderingCoord.X >= Renderer::BufferWidth)
			renderingCoord.X = Renderer::BufferWidth - 1;

		return renderingCoord;
	}


	// General Rendering




	// UI

	// Text

	void UI_Text::Create 
	(
		WString _content, 
		COORD   _startingCell, 
		COORD   _endingCell,
		bool    _shouldCenter
	)
	{
		Content = move(_content);

		RenderCells.resize(Content.size() + 1);

		for (uIntDM cellIndex = 0; cellIndex < Content.size(); cellIndex++)
		{
			RenderCells[cellIndex].Char       = Content[cellIndex];
			RenderCells[cellIndex].Attributes = Console_WhiteCell;
		}

		StartingCell = _startingCell;
		EndingCell   = _endingCell;

		if (_shouldCenter)
		{
			StartingCell.X += (Renderer::BufferWidth / 2) - (SCast<uInt16>(Content.size() + 1) / 2);
			EndingCell  .X += (Renderer::BufferWidth / 2) + (SCast<uInt16>(Content.size() + 1) / 2);

			StartingCell.X--;
			EndingCell  .X--;
		}
		else
		{
			EndingCell.X += SCast<uInt16>(Content.size() + 1);
		}
	}

	void UI_Text::SetToGrey()
	{
		for (auto& cell : RenderCells)
		{
			cell.Attributes.Set(CAttribute::FG_Intensity);
		}
	}

	void UI_Text::SetToWhite()
	{
		for (auto& cell : RenderCells)
		{
			cell.Attributes = Console_WhiteCell;
		}
	}

	void UI_Text::Render()
	{
		WriteToBufferCells(RenderCells.data(), StartingCell, EndingCell);
	}


	// Button

	void UI_Button::Create 
	(
		WString        _text, 
		COORD          _startingCell, 
		COORD          _endingCell, 
		bool           _shouldCenter,
		Void_Function& _callback
	)
	{
		Text.Create(_text, _startingCell, _endingCell, _shouldCenter);

		Callback = getPtr(_callback);
	}

	void UI_Button::Press()
	{
		Callback();
	}

	void UI_Button::Render()
	{
		Text.Render();
	}


	// Grid

	void UI_Grid::Add 
	(
		WString        _text, 
		COORD          _startingCell, 
		COORD          _endingCell, 
		bool           _shouldCenter,
		Void_Function& _callback
	)
	{
		Buttons.push_back(UI_Button());

		Buttons.back().Create(_text, _startingCell, _endingCell, _shouldCenter, _callback);

		if (Buttons.size() != 1)
		{
			Buttons.back().Text.SetToGrey();
		}
	}

	void UI_Grid::MoveUp()
	{
		ptr<UI_Text> buttonText = getPtr(Buttons[CurrentIndex].Text);

		if (CurrentIndex > 0)
		{
			buttonText->SetToGrey();

			CurrentIndex =  CurrentIndex - 1;

			buttonText = getPtr(Buttons[CurrentIndex].Text);

			buttonText->SetToWhite();
		}
	}

	void UI_Grid::MoveDown()
	{
		ptr<UI_Text> buttonText = getPtr(Buttons[CurrentIndex].Text);

		if (CurrentIndex < (Buttons.size() - 1))
		{
			buttonText->SetToGrey();

			CurrentIndex = CurrentIndex + 1;

			buttonText = getPtr(Buttons[CurrentIndex].Text);

			buttonText->SetToWhite();
		}
	}

	void UI_Grid::Select()
	{
		Buttons[CurrentIndex].Press();
	}

	void UI_Grid::Render()
	{
		for (auto& button : Buttons)
		{
			button.Render();
		}
	}


	// Widget

	void UI_Widget::AddText
	(
		WString _text,
		COORD   _startingCell,
		COORD   _endingCell,
		bool    _shouldCenter
	)
	{
		TextUIs.push_back(UI_Text());

		TextUIs.back().Create
		( 
			_text, 
			_startingCell, 
			_endingCell, 
			_shouldCenter
		);
	}

	void UI_Widget::AddButton 
	(
		WString        _text,
		COORD          _startingCell,
		COORD          _endingCell,
		bool           _shouldCenter,
		Void_Function& _callback
	)
	{
		Grid.Add
		( 
			_text, 
			_startingCell, 
			_endingCell, 
			_shouldCenter, 
			_callback
		);
	}

	void UI_Widget::MoveUp()
	{
		Grid.MoveUp();
	}

	void UI_Widget::MoveDown()
	{
		Grid.MoveDown();
	}

	void UI_Widget::Select()
	{
		Grid.Select();
	}

	void UI_Widget::Render()
	{
		for (auto& textUI : TextUIs)
		{
			textUI.Render();
		}

		Grid.Render();
	}
}
