#include <motor/other/Display.h>
#include <motor/other/StringConverter.h>
#include <motor/other/Logs.h>

#include <string>


Display::~Display()
{
	Logs::Debug("Destroy window"); // Тестовый лог
	if (this->hWnd != NULL)
	{
		UnregisterClass(window_class_wide.c_str(), hInstance);
		DestroyWindow(hWnd);
	}
}

bool Display::Initialize(HINSTANCE hInstance, std::string window_class, int width, int height)
{
	Logs::Debug("Initialize raw input for mouse"); // Тестовый лог
	static bool raw_input_initialized = false;
	if (raw_input_initialized == false)
	{
		RAWINPUTDEVICE rid;

		rid.usUsagePage = 0x01; //Mouse
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;

		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			Logs::Error("Failed to register raw input devices.");
			exit(-1);
		}

		raw_input_initialized = true;
	}


	Logs::Debug("Initialize window"); // Тестовый лог

	this->hInstance = hInstance;
	this->window_class_wide = StringConverter::StringToWide(window_class);

	RegisterDisplayClass();

	auto posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	auto posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		window_class_wide.c_str(),
		window_class_wide.c_str(),
		WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME,
		posX,
		posY,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		hInstance,
		this
	);

	if (hWnd == NULL)
	{
		Logs::Error("CreateWindowEX Failed for window: " + window_class);
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	return true;
}

bool Display::ProcessMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); 

	while (PeekMessage(
		&msg,
		hWnd,
		0,   
		0,   
		PM_REMOVE))
	{
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	}

	if (msg.message == WM_NULL)
	{
		if (!IsWindow(hWnd))
		{
			hWnd = NULL;
			UnregisterClass(window_class_wide.c_str(), hInstance);
			return false;
		}
	}

	return true;
}

HWND Display::GetHWND() const
{
	return hWnd;
}

LRESULT CALLBACK WndProcRedirect(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Display* const  pDisplay = reinterpret_cast<Display*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pDisplay->CustomWndProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Display* pDisplay = reinterpret_cast<Display*>(pCreate->lpCreateParams);
		if (pDisplay == nullptr)
		{
			Logs::Error("Critical Error: Pointer to window container is null during WM_NCCREATE.");
			exit(-1);
		}
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pDisplay));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProcRedirect));
		return pDisplay->CustomWndProc(hWnd, msg, wParam, lParam);
	}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

// Custom WndProc
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT Display::CustomWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}

	switch (msg)
	{
	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		return 0;
	}
#pragma region Keyboard_messages
	case WM_KEYDOWN:
	{
		
		unsigned char keycode = static_cast<unsigned char>(wParam);
		if (keyboard.IsKeysAutoRepeat())
		{
			//Logs::Debug("WM_KEYDOWN Autorepeat");
			keyboard.OnKeyPressed(keycode);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				//Logs::Debug("WM_KEYDOWN !Autorepeat");
				keyboard.OnKeyPressed(keycode);
			}
		}
		return 0;
	}
	case WM_KEYUP:
	{
		//Logs::Debug("WM_KEYUP");
		unsigned char keycode = static_cast<unsigned char>(wParam);
		keyboard.OnKeyReleased(keycode);
		return 0;
	}
	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char>(wParam);
		if (keyboard.IsCharsAutoRepeat())
		{
			keyboard.OnChar(ch);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				keyboard.OnChar(ch);
			}
		}
		return 0;
	}
#pragma endregion
#pragma region Mouse_message
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMiddlePressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMiddleReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			mouse.OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			mouse.OnWheelDown(x, y);
		}
		return 0;
	}
	case WM_INPUT:
	{
		UINT dataSize;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER)); //Need to populate data size first

		if (dataSize > 0)
		{
			std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}

		return DefWindowProc(hWnd, msg, wParam, lParam); //Need to call DefWindowProc for WM_INPUT messages
	}
#pragma endregion
	default:
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	}
}

void Display::RegisterDisplayClass()
{
	Logs::Debug("	Register display class"); // Тестовый лог

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProcSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW); // Поменять курсор если потребуется
	wc.hbrBackground = nullptr;
	wc.lpszClassName = window_class_wide.c_str();
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);
}
