#pragma once

namespace UI
{
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