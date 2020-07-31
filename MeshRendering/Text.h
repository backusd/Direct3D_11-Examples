#pragma once

#include "pch.h"
#include <string>
#include <array>
#include "DeviceResources.h"
#include "Enums.h"

using winrt::Windows::Foundation::Point;



namespace UI
{
	// typedef for a function pointer that takes a 'MouseControlStateSetter' parameter
	// and returns a bool value
	//typedef bool (Text::*UpdateIndexFunction)(MouseControlStateSetter);

	//class Text;
	//typedef bool (Text::* UpdateIndexFunction)(MouseControlStateSetter);

	class Text
	{
	public:
		Text(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~Text();

		void Update();
		void Render();
		void ReleaseDeviceDependentResources();

		DWRITE_TEXT_METRICS GetTextMetrics();

		void SetText(std::wstring text);
		void SetFontFamilyName(std::wstring fontFamilyName);

		void SetTextBoxPoint(float x, float y);

		void SetTextColor(					D2D1::ColorF color,										MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);
		void SetTextAlignment(				DWRITE_TEXT_ALIGNMENT textAlignment,					MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);
		void SetParagraphAlignment(			DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment,          MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);

		void SetFontSize(					float fontSize,											MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);
		void SetFontStretch(				DWRITE_FONT_STRETCH fontStretch,						MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);
		void SetFontStyle(					DWRITE_FONT_STYLE fontStyle,							MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);
		void SetFontWeight(					DWRITE_FONT_WEIGHT fontWeight,							MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);

		void SetMaxWidth(					float maxWidth,											MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);
		void SetMaxHeight(					float maxHeight,										MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);

		void SetStrikeThrough(				bool strikeThrough,										MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);
		void SetUnderline(					bool underline,											MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);

		void SetLeadingSpacing(				float leadingSpacing,									MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);
		void SetTrailingSpacing(			float trailingSpacing,									MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);
		void SetMinimumAdvanceWidth(		float minimumAdvanceWidth,								MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);

		void SetLineSpacingMethod(			DWRITE_LINE_SPACING_METHOD lineSpacingMethod,			MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);
		void SetLineSpacingHeight(			float lineSpacingSpacingHeight,							MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);
		void SetLineSpacingBaseline(		float lineSpacingBaseline,								MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);
		void SetLineSpacingLeadingBefore(	float lineSpacingLeadingBefore,							MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);
		void SetLineSpacingFontLineGapUsage(DWRITE_FONT_LINE_GAP_USAGE lineSpacingFontLineGapUsage, MouseControlStateSetter mouseControlStateSetter = MouseControlStateSetter::SET_ALL);


		bool UpdateTheDefaultIndex(MouseControlStateSetter mouseControlStateSetter);
		bool UpdateTheMouseOverIndex(MouseControlStateSetter mouseControlStateSetter);
		bool UpdateTheMouseDownIndex(MouseControlStateSetter mouseControlStateSetter);


	private:
		void CreateTextFormat();
		void CreateTextLayout();
		void CreateDeviceDependentResources();
		void UpdateLineSpacingStruct();

		void InvalidateDeviceDependentResources() { m_deviceDependentResourcesAreValid = false; }
		void InvalidateTextFormat() { m_textFormatIsValid = false; }
		void InvalidateTextLayout() { m_textLayoutIsValid = false; }

		bool CheckUpdateIndexFunction(int index, MouseControlStateSetter mouseControlStateSetter);


		// Array of pointer to the three Update...Index functions
		//std::array<UpdateIndexFunction, 3> M_UpdateIndexFunction;

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// Actual state of the text (will serve as the index into all of the member arrays)
		MouseControlState m_mouseControlState;

		// boolean values describing what is invalid and will need to be created/re-created prior to rendering
		bool m_deviceDependentResourcesAreValid;
		bool m_textFormatIsValid;
		bool m_textLayoutIsValid;

		// Resources related to text rendering.
		std::wstring                                m_text;
		std::wstring								m_fontFamilyName;

		DWRITE_TEXT_METRICS	                        m_textMetrics;
		winrt::com_ptr<ID2D1SolidColorBrush>		m_textColorBrush;
		winrt::com_ptr<ID2D1DrawingStateBlock1>		m_stateBlock;
		winrt::com_ptr<IDWriteTextLayout3>			m_textLayout;
		winrt::com_ptr<IDWriteTextFormat2>			m_textFormat;

		D2D1_POINT_2F								m_textBoxPoint;

		// All text format/layout variables are arrays of 3 variables
		// - The first entry in the array specifies the default value to use
		// - The second entry in the array specifies the value to use when the mouse is over the text or containing object/control
		// - The third entry in the array specifies the value to use when the mouse is clicked down when over the text or containing object/control
		std::array<D2D1::ColorF, 3> m_textColor;

		// IDWriteTextFormat specific variables
		std::array<DWRITE_TEXT_ALIGNMENT, 3>	  m_textAlignment;
		std::array<DWRITE_PARAGRAPH_ALIGNMENT, 3> m_paragraphAlignment;

		// IDWriteTextFormat1 specific variables
		//  - There are some methods that could be used, but would provide functionality that is not yet needed by this app

		// IDWriteTextFormat2 specific variables
		//  - There are some methods that could be used, but would provide functionality that is not yet needed by this app

		// IDWriteTextLayout specific variables
		std::array<float, 3>				m_fontSize;
		std::array<DWRITE_FONT_STRETCH, 3>	m_fontStretch;
		std::array<DWRITE_FONT_STYLE, 3>	m_fontStyle;
		std::array<DWRITE_FONT_WEIGHT, 3>	m_fontWeight;

		std::array<float, 3> m_maxWidth;
		std::array<float, 3> m_maxHeight;

		std::array<bool, 3> m_strikeThrough;
		std::array<bool, 3> m_underline;

		// IDWriteTextLayout1 specific variables
		std::array<float, 3> m_leadingSpacing;		// The spacing before each character, in reading order
		std::array<float, 3> m_trailingSpacing;		// The spacing after each character, in reading order
		std::array<float, 3> m_minimumAdvanceWidth; // The minimum advance of each character to prevent characters from becoming too thin or zero-width. Must be >= 0

		// IDWriteTextLayout2 specific variables
		//  - There are some methods that could be used, but would provide functionality that is not yet needed by this app

		// IDWriteTextLayout3 specific variables
		DWRITE_LINE_SPACING						  m_lineSpacingStruct;				// struct that holds all of the line spacing details
		std::array<DWRITE_LINE_SPACING_METHOD, 3> m_lineSpacingMethod;				// options are DEFAULT, UNIFORM, or PROPORTIONAL
		std::array<float, 3>					  m_lineSpacingHeight;				// interpretation depends on the line spacing method, but this essentially sets the spacing between each line
		std::array<float, 3>					  m_lineSpacingBaseline;			// interpretation depends on the line spacing method, but this essentially sets the distance from the top of the line to baseline
		std::array<float, 3>					  m_lineSpacingLeadingBefore;		// only used when line spacing method is PROPORTIONAL - value must be between 0-1.0
		std::array<DWRITE_FONT_LINE_GAP_USAGE, 3> m_lineSpacingFontLineGapUsage;	// specifies whether or not lineGap is part of the line metrics


	};
}
