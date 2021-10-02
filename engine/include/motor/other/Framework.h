#pragma once

#include <motor/other/Display.h>
#include <motor/graphics/Graphics.h>
#include <motor/other/Timer.h>

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