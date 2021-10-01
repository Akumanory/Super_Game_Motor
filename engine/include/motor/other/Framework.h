#pragma once
#include "Display.h"
#include "Graphics.h"
#include "Timer.h";

class Framework
{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_class, int width, int height);
	bool ProcessMessages();
	void Update();
	void RenderFrame();
private:
	Timer timer;
	Display display;
	Graphics gfx;
};