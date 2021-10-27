#include <motor/other/DirectXIncludes.h>
#include <motor/other/Logs.h>
#include <motor/other/Framework.h>

int WINAPI main(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR lpCmdLine,
  int nCmdShow) 
{
    Logs::Debug("Main call"); // Тестовый лог

    Framework framework;

    if (framework.Initialize(hInstance, "Test window", 1240, 720)) {
        Logs::Debug("Start ProcessMessage"); // Тестовый лог
        while (framework.ProcessMessages()) {
            framework.Update();
            framework.RenderFrame();
        }
        Logs::Debug("End ProcessMessage"); // Тестовый лог
    }

    return 0;
};
