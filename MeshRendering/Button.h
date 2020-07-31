#pragma once

#include "RectangularControl.h"
#include "Text.h"

namespace UI
{
	class Button : public RectangularControl
	{
	public:
		Button(	const std::shared_ptr<DX::DeviceResources>& deviceResources,
				Point topLeft,
				float height,
				float width,
				HeightWidthType heightType,
				HeightWidthType widthType
		);
		Button(const std::shared_ptr<DX::DeviceResources>& deviceResources,
			float top,
			float left,
			float height,
			float width,
			HeightWidthType heightType,
			HeightWidthType widthType
		);

		void Render(float windowHeight, float windowWidth);


	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		std::unique_ptr<Text> m_text;



	};
}
