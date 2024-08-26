#pragma once

#include "hzpch.h"
#include "Events/Event.h"

#include "Core.h"

namespace Hazel
{
	struct WindowProps
	{
		unsigned int Width, Height;
		std::string Title;

		WindowProps(const std::string title = "Hazel Window", unsigned int width = 1280, unsigned int height = 720)
			: Title(title), Width(width), Height(height) {}
	};


	// Interface representing a desktop system based Window
	class HAZEL_API Window
	{
	public:
		using EventCallBackFn = std::function<void(Event&)>;

		virtual ~Window() {};

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallBack(const EventCallBackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}