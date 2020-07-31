#pragma once

namespace UI
{
	enum class HeightWidthType {
		FIXED_PIXELS,	// height/width value will be fixed to the value of pixels regardless of window re-sizing
		PERCENTAGE		// height/width value will be a percentage of total window height/width
	};

	enum MouseControlState
	{
		DEFAULT = 0,
		MOUSE_OVER = 1,
		MOUSE_DOWN = 2
	};

	enum class MouseControlStateSetter
	{
		SET_DEFAULT,
		SET_MOUSE_OVER,
		SET_MOUSE_DOWN,
		SET_DEFAULT_AND_MOUSE_OVER,
		SET_DEFAULT_AND_MOUSE_DOWN,
		SET_MOUSE_OVER_AND_MOUSE_DOWN,
		SET_ALL
	};

}