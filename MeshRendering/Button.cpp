#include "pch.h"
#include "Button.h"


namespace UI
{
	Button::Button(
		const std::shared_ptr<DX::DeviceResources>& deviceResources,
		Point topLeft,
		float height,
		float width,
		HeightWidthType heightType,
		HeightWidthType widthType
	) :
		RectangularControl(topLeft, height, width, heightType, widthType),
		m_deviceResources(deviceResources)
	{
		m_text = std::unique_ptr<Text>(new Text(m_deviceResources));
	}
	Button::Button(
		const std::shared_ptr<DX::DeviceResources>& deviceResources,
		float top,
		float left,
		float height,
		float width,
		HeightWidthType heightType,
		HeightWidthType widthType
	) :
		RectangularControl(top, left, height, width, heightType, widthType),
		m_deviceResources(deviceResources)
	{
		m_text = std::unique_ptr<Text>(new Text(m_deviceResources));


		m_text->SetText(L"Some Happy Text");
		m_text->SetTextColor(D2D1::ColorF::White);

	}

	void Button::Render(float windowHeight, float windowWidth)
	{
		m_text->Render();
	}

}