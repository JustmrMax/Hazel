#pragma once

extern Hazel::Application* Hazel::CreateApplication();

int main(int* args, int** argv)
{
	auto application = Hazel::CreateApplication();
	application->Run();
	delete application;
}