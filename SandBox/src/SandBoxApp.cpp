#include "SandBoxApp.h"

#include "glm/mat4x4.hpp"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		glm::mat4x4 projectionMatrix = glm::mat4x4();
		int x;
	}

	void OnUpdate()
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
			HZ_TRACE("Tab key is Pressed!");


	}

	void OnEvent(Hazel::Event& event)
	{
		// HZ_CORE_TRACE(event.ToString());
		if (event.GetEventType() == Hazel::EventType::KeyPressed)
		{
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			HZ_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

SandBox::SandBox()
{
	PushLayer(new ExampleLayer());
	PushLayer(new Hazel::ImGuiLayer());
}

SandBox::~SandBox()
{

}

Hazel::Application* Hazel::CreateApplication()
{
	return new SandBox();
}