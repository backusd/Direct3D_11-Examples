#include "pch.h"
#include "Text.h"
#include "DirectXHelper.h"



namespace UI
{
	Text::Text(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
		//M_UpdateIndexFunction({ &Text::UpdateTheDefaultIndex, &Text::UpdateTheMouseOverIndex, &Text::UpdateTheMouseDownIndex }),
		m_deviceResources(deviceResources),
		m_mouseControlState(MouseControlState::DEFAULT),
		m_deviceDependentResourcesAreValid(false),
		m_textFormatIsValid(false),
		m_textLayoutIsValid(false),
		m_text(L""),
		m_fontFamilyName(L"Segoe UI"),
		m_textColor({ D2D1::ColorF(D2D1::ColorF::Black), D2D1::ColorF(D2D1::ColorF::Black), D2D1::ColorF(D2D1::ColorF::Black) }),
		m_textAlignment({ DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING , DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING }),
		m_paragraphAlignment({ DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR, DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR , DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR }),
		m_fontSize({ 20.0f, 20.0f, 20.0f }),
		m_fontStretch({ DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL, DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL, DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL }),
		m_fontStyle({ DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL }),
		m_fontWeight({ DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL }),
		m_maxWidth({ 1000.0f, 1000.0f, 1000.0f }),
		m_maxHeight({ 1000.0f, 1000.0f, 1000.0f }),
		m_strikeThrough({ false, false, false }),
		m_underline({ false, false, false }),
		m_leadingSpacing({ 0.0f, 0.0f, 0.0f }),
		m_trailingSpacing({ 0.0f, 0.0f, 0.0f }),
		m_minimumAdvanceWidth({ 0.0f, 0.0f, 0.0f }),
		m_lineSpacingMethod({ DWRITE_LINE_SPACING_METHOD::DWRITE_LINE_SPACING_METHOD_DEFAULT, DWRITE_LINE_SPACING_METHOD::DWRITE_LINE_SPACING_METHOD_DEFAULT , DWRITE_LINE_SPACING_METHOD::DWRITE_LINE_SPACING_METHOD_DEFAULT }),
		m_lineSpacingHeight({ 0.0f, 0.0f, 0.0f }),
		m_lineSpacingBaseline({ 0.0f, 0.0f, 0.0f }),
		m_lineSpacingLeadingBefore({ 0.0f, 0.0f, 0.0f }),
		m_lineSpacingFontLineGapUsage({ DWRITE_FONT_LINE_GAP_USAGE::DWRITE_FONT_LINE_GAP_USAGE_DEFAULT, DWRITE_FONT_LINE_GAP_USAGE::DWRITE_FONT_LINE_GAP_USAGE_DEFAULT, DWRITE_FONT_LINE_GAP_USAGE::DWRITE_FONT_LINE_GAP_USAGE_DEFAULT })
	{
		m_textBoxPoint.x = 0.0f;
		m_textBoxPoint.y = 0.0f;

		ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));
		UpdateLineSpacingStruct();
		CreateTextFormat();
		CreateDeviceDependentResources();
	}
	Text::~Text()
	{
		ReleaseDeviceDependentResources();
	}

	void Text::CreateTextFormat()
	{
		m_textFormat = nullptr;
		m_stateBlock = nullptr;
		
		// Create device independent resources
		winrt::com_ptr<IDWriteTextFormat> textFormat;
		DX::ThrowIfFailed(
			m_deviceResources->GetDWriteFactory()->CreateTextFormat(
				m_fontFamilyName.c_str(),
				nullptr,
				m_fontWeight[m_mouseControlState],
				m_fontStyle[m_mouseControlState],
				m_fontStretch[m_mouseControlState],
				m_fontSize[m_mouseControlState],
				L"en-US",
				textFormat.put()
			)
		);

		DX::As(textFormat, m_textFormat);

		// Aligns the text vertically in the layout box
		DX::ThrowIfFailed(
			m_textFormat->SetParagraphAlignment(m_paragraphAlignment[m_mouseControlState])
		);

		// Aligns the text horizontally in the layout box
		DX::ThrowIfFailed(
			m_textFormat->SetTextAlignment(m_textAlignment[m_mouseControlState])
		);

		// Create the state block used in drawing
		DX::ThrowIfFailed(
			m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock(m_stateBlock.put())
		);

		m_textFormatIsValid = true;
	}
	void Text::CreateDeviceDependentResources()
	{
		// Create the color brush for the text color
		DX::ThrowIfFailed(
			m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(m_textColor[m_mouseControlState], m_textColorBrush.put())
		);

		m_deviceDependentResourcesAreValid = true;
	}
	void Text::CreateTextLayout()
	{
		ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));
		m_textLayout = nullptr;

		winrt::com_ptr<IDWriteTextLayout> textLayout;
		DX::ThrowIfFailed(
			m_deviceResources->GetDWriteFactory()->CreateTextLayout(
				m_text.c_str(),
				(uint32_t)m_text.length(),
				m_textFormat.get(),
				m_maxWidth[m_mouseControlState], // Max width of the input text.
				m_maxHeight[m_mouseControlState], // Max height of the input text.
				textLayout.put()
			)
		);

		DX::As(textLayout, m_textLayout);

		DX::ThrowIfFailed(
			m_textLayout->GetMetrics(&m_textMetrics)
		);

		m_textLayoutIsValid = true;
	}
	void Text::Update()
	{
		if (!m_textFormatIsValid) CreateTextFormat();
		if (!m_deviceDependentResourcesAreValid) CreateDeviceDependentResources();
		if (!m_textLayoutIsValid) CreateTextLayout();
	}
	void Text::Render()
	{
		ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
		//auto logicalSize = m_deviceResources->GetLogicalSize();

		context->SaveDrawingState(m_stateBlock.get());
		context->BeginDraw();

		// Position on the bottom right corner
		/*
		D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
			logicalSize.Width - m_textMetrics.layoutWidth,
			logicalSize.Height - m_textMetrics.height
		);

		context->SetTransform(screenTranslation * m_deviceResources->GetOrientationTransform2D());
		*/
		
		context->DrawTextLayout(
			m_textBoxPoint,
			m_textLayout.get(),
			m_textColorBrush.get()
		);

		// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
		// is lost. It will be handled during the next call to Present.
		HRESULT hr = context->EndDraw();
		if (hr != D2DERR_RECREATE_TARGET)
		{
			DX::ThrowIfFailed(hr);
		}

		context->RestoreDrawingState(m_stateBlock.get());
	}

	DWRITE_TEXT_METRICS Text::GetTextMetrics()
	{
		Update();
		return m_textMetrics;
	}

	void Text::SetText(std::wstring text)
	{
		if (m_text.compare(text) != 0)
		{
			m_text = text;
			InvalidateTextLayout();
		}
	}
	void Text::SetFontFamilyName(std::wstring fontFamilyName)
	{
		if (m_fontFamilyName.compare(fontFamilyName) != 0)
		{
			m_fontFamilyName = fontFamilyName;
			InvalidateTextFormat();
		}
	}

	void Text::SetTextBoxPoint(float x, float y)
	{
		m_textBoxPoint.x = x;
		m_textBoxPoint.y = y;
	}

	void Text::SetTextColor(D2D1::ColorF color, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			//UpdateIndexFunction Fn = M_UpdateIndexFunction[iii];
			//M_UpdateIndexFunction[iii](mouseControlStateSetter)
			// Syntax explanation:
			// 1. Retrieve the ith function pointer from the array
			// 2. Call this-> on the deferenced function pointer
			//if ((this->*M_UpdateIndexFunction[iii])(mouseControlStateSetter) && !DX::ColorsAreEqual(m_textColor[iii], color))
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && !DX::ColorsAreEqual(m_textColor[iii], color))
			{
				m_textColor[iii] = color;
				InvalidateDeviceDependentResources();
			}
		}
	}
	void Text::SetTextAlignment(DWRITE_TEXT_ALIGNMENT textAlignment, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_textAlignment[iii] != textAlignment)
			{
				m_textAlignment[iii] = textAlignment;
				InvalidateTextFormat();
			}
		}
	}
	void Text::SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_paragraphAlignment[iii] != paragraphAlignment)
			{
				m_paragraphAlignment[iii] = paragraphAlignment;
				InvalidateTextFormat();
			}
		}
	}

	void Text::SetFontSize(float fontSize, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_fontSize[iii] != fontSize)
			{
				m_fontSize[iii] = fontSize;
				InvalidateTextFormat();
			}
		}
	}
	void Text::SetFontStretch(DWRITE_FONT_STRETCH fontStretch, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_fontStretch[iii] != fontStretch)
			{
				m_fontStretch[iii] = fontStretch;
				InvalidateTextFormat();
			}
		}
	}
	void Text::SetFontStyle(DWRITE_FONT_STYLE fontStyle, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_fontStyle[iii] != fontStyle)
			{
				m_fontStyle[iii] = fontStyle;
				InvalidateTextFormat();
			}
		}
	}
	void Text::SetFontWeight(DWRITE_FONT_WEIGHT fontWeight, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_fontWeight[iii] != fontWeight)
			{
				m_fontWeight[iii] = fontWeight;
				InvalidateTextFormat();
			}
		}
	}

	void Text::SetMaxWidth(float maxWidth, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_maxWidth[iii] != maxWidth)
			{
				m_maxWidth[iii] = maxWidth;
				InvalidateTextLayout();
			}
		}
	}
	void Text::SetMaxHeight(float maxHeight, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_maxHeight[iii] != maxHeight)
			{
				m_maxHeight[iii] = maxHeight;
				InvalidateTextLayout();
			}
		}
	}

	void Text::SetStrikeThrough(bool strikeThrough, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_strikeThrough[iii] != strikeThrough)
			{
				m_strikeThrough[iii] = strikeThrough;
				InvalidateTextLayout();
			}
		}
	}
	void Text::SetUnderline(bool underline, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_underline[iii] != underline)
			{
				m_underline[iii] = underline;
				InvalidateTextLayout();
			}
		}
	}

	void Text::SetLeadingSpacing(float leadingSpacing, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_leadingSpacing[iii] != leadingSpacing)
			{
				m_leadingSpacing[iii] = leadingSpacing;
				InvalidateTextLayout();
			}
		}
	}
	void Text::SetTrailingSpacing(float trailingSpacing, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_trailingSpacing[iii] != trailingSpacing)
			{
				m_trailingSpacing[iii] = trailingSpacing;
				InvalidateTextLayout();
			}
		}
	}
	void Text::SetMinimumAdvanceWidth(float minimumAdvanceWidth, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_minimumAdvanceWidth[iii] != minimumAdvanceWidth)
			{
				m_minimumAdvanceWidth[iii] = minimumAdvanceWidth;
				InvalidateTextLayout();
			}
		}
	}

	void Text::SetLineSpacingMethod(DWRITE_LINE_SPACING_METHOD lineSpacingMethod, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_lineSpacingMethod[iii] != lineSpacingMethod)
			{
				m_lineSpacingMethod[iii] = lineSpacingMethod;
				UpdateLineSpacingStruct();
				InvalidateTextLayout();
			}
		}
	}
	void Text::SetLineSpacingHeight(float lineSpacingSpacingHeight, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_lineSpacingHeight[iii] != lineSpacingSpacingHeight)
			{
				m_lineSpacingHeight[iii] = lineSpacingSpacingHeight;
				UpdateLineSpacingStruct();
				InvalidateTextLayout();
			}
		}
	}
	void Text::SetLineSpacingBaseline(float lineSpacingBaseline, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_lineSpacingBaseline[iii] != lineSpacingBaseline)
			{
				m_lineSpacingBaseline[iii] = lineSpacingBaseline;
				UpdateLineSpacingStruct();
				InvalidateTextLayout();
			}
		}
	}
	void Text::SetLineSpacingLeadingBefore(float lineSpacingLeadingBefore, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_lineSpacingLeadingBefore[iii] != lineSpacingLeadingBefore)
			{
				m_lineSpacingLeadingBefore[iii] = lineSpacingLeadingBefore;
				UpdateLineSpacingStruct();
				InvalidateTextLayout();
			}
		}
	}
	void Text::SetLineSpacingFontLineGapUsage(DWRITE_FONT_LINE_GAP_USAGE lineSpacingFontLineGapUsage, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL)
	{
		// loop over each update index function and check what needs updating
		for (int iii = 0; iii < 3; ++iii)
		{
			if (CheckUpdateIndexFunction(iii, mouseControlStateSetter) && m_lineSpacingFontLineGapUsage[iii] != lineSpacingFontLineGapUsage)
			{
				m_lineSpacingFontLineGapUsage[iii] = lineSpacingFontLineGapUsage;
				UpdateLineSpacingStruct();
				InvalidateTextLayout();
			}
		}
	}











	bool Text::UpdateTheDefaultIndex(MouseControlStateSetter mouseControlStateSetter)
	{
		return mouseControlStateSetter == MouseControlStateSetter::SET_ALL ||
			   mouseControlStateSetter == MouseControlStateSetter::SET_DEFAULT ||
			   mouseControlStateSetter == MouseControlStateSetter::SET_DEFAULT_AND_MOUSE_DOWN ||
			   mouseControlStateSetter == MouseControlStateSetter::SET_DEFAULT_AND_MOUSE_OVER;
	}
	bool Text::UpdateTheMouseOverIndex(MouseControlStateSetter mouseControlStateSetter)
	{
		return mouseControlStateSetter == MouseControlStateSetter::SET_ALL ||
			   mouseControlStateSetter == MouseControlStateSetter::SET_MOUSE_OVER ||
			   mouseControlStateSetter == MouseControlStateSetter::SET_DEFAULT_AND_MOUSE_OVER ||
			   mouseControlStateSetter == MouseControlStateSetter::SET_MOUSE_OVER_AND_MOUSE_DOWN;
	}
	bool Text::UpdateTheMouseDownIndex(MouseControlStateSetter mouseControlStateSetter)
	{
		return mouseControlStateSetter == MouseControlStateSetter::SET_ALL ||
			   mouseControlStateSetter == MouseControlStateSetter::SET_MOUSE_DOWN ||
			   mouseControlStateSetter == MouseControlStateSetter::SET_DEFAULT_AND_MOUSE_DOWN ||
			   mouseControlStateSetter == MouseControlStateSetter::SET_MOUSE_OVER_AND_MOUSE_DOWN;
	}

	bool Text::CheckUpdateIndexFunction(int index, MouseControlStateSetter mouseControlStateSetter)
	{
		switch (index)
		{
		case 0: return UpdateTheDefaultIndex(mouseControlStateSetter);
		case 1: return UpdateTheMouseOverIndex(mouseControlStateSetter);
		case 2: return UpdateTheMouseDownIndex(mouseControlStateSetter);
		default:
			winrt::check_bool(false);	// Throw an error as this is not an acceptable value
			return false;				// Include this line so compiler doesn't complain
		}
	}

	void Text::UpdateLineSpacingStruct()
	{
		m_lineSpacingStruct.method = m_lineSpacingMethod[m_mouseControlState];
		m_lineSpacingStruct.height = m_lineSpacingHeight[m_mouseControlState];
		m_lineSpacingStruct.baseline = m_lineSpacingBaseline[m_mouseControlState];
		m_lineSpacingStruct.leadingBefore = m_lineSpacingLeadingBefore[m_mouseControlState];
		m_lineSpacingStruct.fontLineGapUsage = m_lineSpacingFontLineGapUsage[m_mouseControlState];
	}

	void Text::ReleaseDeviceDependentResources()
	{
		m_textColorBrush = nullptr;
	}
}