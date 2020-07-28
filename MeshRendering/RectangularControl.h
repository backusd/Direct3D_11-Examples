#pragma once

#include "Control.h"

using winrt::Windows::Foundation::Point;


namespace UI
{
	enum class HeightWidthType {
		FIXED_PIXELS,	// height/width value will be fixed to the value of pixels regardless of window re-sizing
		PERCENTAGE		// height/width value will be a percentage of total window height/width
	};

	class RectangularControl : public Control
	{
	public:
		// Constructors
		RectangularControl(
			const std::shared_ptr<UI::AppLayout> appLayout,
			Point topLeft,
			float height,
			float width,
			HeightWidthType heightType,
			HeightWidthType widthType
		);
		RectangularControl(
			const std::shared_ptr<UI::AppLayout> appLayout,
			float top,
			float left,
			float height,
			float width,
			HeightWidthType heightType,
			HeightWidthType widthType
		);

		// GET Methods
		float HeightInPixels();
		float WidthInPixels();

		// SET Methods
		void TopLeft(Point topLeft);
		void TopLeft(float top, float left);

	private:
		void ValidateHeightWidthValues();

		// Pointer to AppLayout - required for determining overall window size
		std::shared_ptr<UI::AppLayout> m_appLayout;

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

