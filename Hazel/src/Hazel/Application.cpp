#include "hzprh.h"
#include "Application.h"

#include "Events/MouseEvent.h"
#include "Log.h"

namespace Hazel
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		MouseButtonPressedEvent e(19);

		if (e.IsInCategory(EventCategoryMouse))
			HZ_CORE_TRACE(e.ToString());
		if (e.IsInCategory(EventCategoryInput))
			HZ_CORE_TRACE(e.ToString());

		while (true);
	}
}