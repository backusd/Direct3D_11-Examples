#include "pch.h"
#include "RectangularControl.h"


namespace UI
{
	RectangularControl::RectangularControl(
		const std::shared_ptr<UI::AppLayout> appLayout,
		Point topLeft,
		float height,
		float width,
		HeightWidthType heightType,
		HeightWidthType widthType
	) :
		m_appLayout(appLayout),
		m_topLeft(topLeft),
		m_height(height),
		m_width(width),
		m_heightType(heightType),
		m_widthType(widthType)
	{
		ValidateHeightWidthValues();
	}

	RectangularControl::RectangularControl(
		const std::shared_ptr<UI::AppLayout> appLayout,
		float top,
		float left,
		float height,
		float width,
		HeightWidthType heightType,
		HeightWidthType widthType
	) :
		m_appLayout(appLayout),
		m_height(height),
		m_width(width),
		m_heightType(heightType),
		m_widthType(widthType)
	{
		m_topLeft.X = left;
		m_topLeft.Y = top;

		ValidateHeightWidthValues();
	}

	void RectangularControl::ValidateHeightWidthValues()
	{
		// implement some form of height/width validation checking
	}

	float RectangularControl::HeightInPixels()
	{
		switch (m_heightType)
		{
		case HeightWidthType::FIXED_PIXELS: return m_height;
		case HeightWidthType::PERCENTAGE:	return m_height * m_appLayout->WindowHeight();
		default:
			winrt::check_bool(false);	// Throw an error as this is not an acceptable value
			return -1.0f;				// Include this line so compiler doesn't complain
		}
	}
	float RectangularControl::WidthInPixels()
	{
		switch (m_widthType)
		{
		case HeightWidthType::FIXED_PIXELS: return m_width;
		case HeightWidthType::PERCENTAGE:	return m_width * m_appLayout->WindowWidth();
		default:
			winrt::check_bool(false);	// Throw an error as this is not an acceptable value
			return -1.0f;				// Include this line so compiler doesn't complain
		}
	}

	void RectangularControl::TopLeft(Point topLeft)
	{
		m_topLeft = topLeft;
	}
	void RectangularControl::TopLeft(float top, float left)
	{
		m_topLeft.X = left;
		m_topLeft.Y = top;
	}
}