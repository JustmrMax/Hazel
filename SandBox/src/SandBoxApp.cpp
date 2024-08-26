#include "SandBoxApp.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnUpdate() override
	{
		HZ_INFO("ExampleLayer::Update");
	}

	void OnEvent(Hazel::Event& event) override
	{
		HZ_CORE_TRACE(event.ToString());
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