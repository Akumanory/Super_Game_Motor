#include <motor/core_system/core.hpp>
#include <motor/utils.hpp>
#include <motor_editor/UI/EditorUI.hpp>

using namespace motor;

int WINAPI WinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPSTR lpCmdLine,
  _In_ int nShowCmd) {

    auto handle = GetCurrentThread();
    SetThreadAffinityMask(handle, 1);

#if _DEBUG
    utils::enable_console();
#endif // _DEBUG

    core core{ "Motor editor", hInstance };
    core.setUI<motor_editor::UI::EditorUI>();
    core.run();

    return 0;
}