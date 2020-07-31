#include "pch.h"
#include "AppLayout.h"


namespace UI
{
	AppLayout::AppLayout(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
		m_deviceResources(deviceResources),
		m_windowClosed(false),
		m_windowVisible(true)
	{
		m_main = std::unique_ptr<MeshRendering::Main>(new MeshRendering::Main(m_deviceResources));

		m_button = std::unique_ptr<UI::Button>(
			new UI::Button(
				m_deviceResources,
				0.0f,
				0.0f,
				100.0f,
				100.0f,
				UI::HeightWidthType::FIXED_PIXELS,
				UI::HeightWidthType::FIXED_PIXELS
			)
		);
	}

	bool AppLayout::Render()
	{
		m_main->Render();
		m_button->Render(0, 0);

		return true;
	}

	void AppLayout::Run()
	{
		while (!m_windowClosed)
		{
			if (m_windowVisible)
			{
				CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

				m_main->Update();
				if (Render())
				{
					m_deviceResources->Present();
				}
			}
			else
			{
				CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
			}
		}
	}

	void AppLayout::CreateWindowSizeDependentResources()
	{
		m_main->CreateWindowSizeDependentResources();
	}
	void AppLayout::Suspend()
	{
		m_main->Suspend();
	}
	void AppLayout::Resume()
	{
		m_main->Resume();
	}
	void AppLayout::Visibility(bool visibility)
	{
		m_windowVisible = visibility;
	}
	void AppLayout::WindowActivationChanged(CoreWindowActivationState activationState)
	{
		m_main->WindowActivationChanged(activationState);
	}
}