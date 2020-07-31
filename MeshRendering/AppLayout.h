#pragma once

#include "Button.h"
#include "DeviceResources.h"
#include "Main.h"
#include "Enums.h"

namespace UI
{
	class AppLayout
	{
	public:
		AppLayout(const std::shared_ptr<DX::DeviceResources>& deviceResources);

		bool Render();
		void CreateWindowSizeDependentResources();

		// App Lifecycle Functions - Should only be called from App.cpp
		void Run();
		void Suspend();
		void Resume();
		void Visibility(bool visibility);
		void Close() { m_windowClosed = true; }
		void WindowActivationChanged(CoreWindowActivationState activationState);


	private:
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		std::unique_ptr<UI::Button> m_button;
		std::unique_ptr<MeshRendering::Main> m_main;

		bool m_windowVisible;
		bool m_windowClosed;
	};
}
