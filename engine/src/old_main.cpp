#include <motor/other/DirectXIncludes.h>
#include <motor/other/Logs.h>
#include <motor/other/Framework.h>

int WINAPI main(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR lpCmdLine,
  int nCmdShow) {
    Logs::Debug("Main call"); // �������� ���

    Framework framework;

    if (framework.Initialize(hInstance, "Test window", 1600, 900)) {
        Logs::Debug("Start ProcessMessage"); // �������� ���
        while (framework.ProcessMessages()) {
            framework.Update();
            framework.RenderFrame();
        }
        Logs::Debug("End ProcessMessage"); // �������� ���
    }

    return 0;
};
