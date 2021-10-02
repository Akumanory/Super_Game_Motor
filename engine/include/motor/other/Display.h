﻿#pragma once

#include <motor/other/DirectXIncludes.h>
#include <motor/keyboard/KeyboardLayout.h>
#include <motor/mouse/MouseLayout.h>

class Display
{
public:
	~Display();

	bool Initialize(HINSTANCE hInstance, std::string window_class, int width, int height);
	bool ProcessMessages();
	HWND GetHWND() const;
	LRESULT CustomWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	KeyboardLayout keyboard;
	MouseLayout mouse;
private:
	void RegisterDisplayClass();
	HWND hWnd = NULL; 
	HINSTANCE hInstance = NULL; 
	std::wstring window_class_wide = L""; 
};