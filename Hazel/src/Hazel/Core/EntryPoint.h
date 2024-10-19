#pragma once

extern Hazel::Application* Hazel::CreateApplication();

int main(int* args, int** argv)
{
	//Init Logging system
	Hazel::Log::Init();

	HZ_CORE_WARN("Initialize log!");
	HZ_INFO("App");

	auto application = Hazel::CreateApplication();
	application->Run();
	delete application;
}