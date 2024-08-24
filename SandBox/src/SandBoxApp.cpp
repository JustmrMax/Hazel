#include "SandBoxApp.h"

SandBox::SandBox()
{

}

SandBox::~SandBox()
{

}

Hazel::Application* Hazel::CreateApplication()
{
	return new SandBox();
}