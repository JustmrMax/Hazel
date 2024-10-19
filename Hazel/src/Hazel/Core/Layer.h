#pragma once

#include <string>

#include "Core.h"
#include "Hazel/Events/Event.h"
#include "Timestep.h"

namespace Hazel
{
	// Base Class
	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDettach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event &event) {}

		inline const std::string& GetName() { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}