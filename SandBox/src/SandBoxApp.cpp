#include "SandBoxApp.h"

#include "imgui.h" 

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	virtual void OnUpdate() override
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
			HZ_TRACE("Tab key is Pressed!");
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World!");
		ImGui::End();
	}

	virtual void OnEvent(Hazel::Event& event) override
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
}

SandBox::~SandBox()
{

}

Hazel::Application* Hazel::CreateApplication()
{
	return new SandBox();
}