#pragma once

#include "Control.h"
#include "Enums.h"

using winrt::Windows::Foundation::Point;

namespace UI
{
	class RectangularControl : public Control
	{
	public:
		// Constructors
		RectangularControl(
			Point topLeft,
			float height,
			float width,
			HeightWidthType heightType,
			HeightWidthType widthType
		);
		RectangularControl(
			float top,
			float left,
			float height,
			float width,
			HeightWidthType heightType,
			HeightWidthType widthType
		);

		// GET Methods
		float HeightInPixels(float windowHeight);
		float WidthInPixels(float windowWidth);

		// SET Methods
		void TopLeft(Point topLeft);
		void TopLeft(float top, float left);

	private:
		void ValidateHeightWidthValues();

		// Top Left point of the control
		Point m_topLeft;

		// Height and width of the control - will be interpreted based on HeightWidthType values
		float m_height;
		float m_width;

		// Height/width type used to determine interpretation of height/width values
		HeightWidthType m_heightType;
		HeightWidthType m_widthType;
		
	};
}

