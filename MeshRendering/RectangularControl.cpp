#include "pch.h"
#include "RectangularControl.h"


namespace UI
{
	RectangularControl::RectangularControl(
		Point topLeft,
		float height,
		float width,
		HeightWidthType heightType,
		HeightWidthType widthType
	) :
		m_topLeft(topLeft),
		m_height(height),
		m_width(width),
		m_heightType(heightType),
		m_widthType(widthType)
	{
		ValidateHeightWidthValues();
	}

	RectangularControl::RectangularControl(
		float top,
		float left,
		float height,
		float width,
		HeightWidthType heightType,
		HeightWidthType widthType
	) :
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

	float RectangularControl::HeightInPixels(float windowHeight)
	{
		switch (m_heightType)
		{
		case HeightWidthType::FIXED_PIXELS: return m_height;
		case HeightWidthType::PERCENTAGE:	return m_height * windowHeight;
		default:
			winrt::check_bool(false);	// Throw an error as this is not an acceptable value
			return -1.0f;				// Include this line so compiler doesn't complain
		}
	}
	float RectangularControl::WidthInPixels(float windowWidth)
	{
		switch (m_widthType)
		{
		case HeightWidthType::FIXED_PIXELS: return m_width;
		case HeightWidthType::PERCENTAGE:	return m_width * windowWidth;
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