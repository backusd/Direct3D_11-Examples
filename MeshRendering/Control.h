#pragma once

#include "pch.h"
#include "AppLayout.h"

using winrt::Windows::Foundation::EventHandler;

namespace UI
{
	class Control
	{
	public:
		winrt::delegate<> MouseEntered;
		winrt::delegate<> MouseExited;

		winrt::delegate<> MouseDown;
		winrt::delegate<> MouseUp;

	private:

	};
}