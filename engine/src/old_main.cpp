#include <motor/other/DirectXIncludes.h>
#include <motor/other/Logs.h>
#include <motor/other/Framework.h>
#include <motor/other/Instrumentor.h>

int WINAPI main(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR lpCmdLine,
  int nCmdShow) 
{
    Logs::Debug("Main call"); // Тестовый лог

    Framework framework;

    Instrumentor::Get().BeginSession("Framework::Initialize", "Performance/Framework_Initialize.json");
    if (framework.Initialize(hInstance, "Test window", 2000, 1200)) 
    {
        Instrumentor::Get().EndSession();

        Logs::Debug("Start ProcessMessage"); // Тестовый лог
        while (framework.ProcessMessages()) {
            framework.Update();
            framework.RenderFrame();
        }
        Logs::Debug("End ProcessMessage"); // Тестовый лог
    } else 
    {
        Instrumentor::Get().EndSession();
    }

    return 0;
};
