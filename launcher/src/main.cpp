#include <motor/core_system/core.hpp>
#include <motor/utils.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace motor;

int WINAPI WinMain(
  _In_ HINSTANCE h_instance,
  _In_opt_ HINSTANCE h_prev_instance,
  _In_ LPSTR lp_cmd_line,
  _In_ int n_show_cmd) {

#if _DEBUG
    utils::enable_console();
#endif // _DEBUG

    utils::debug_write::info("launcher is starting\n");

    core_system::core().run();

    utils::debug_write::info("launcher is shutting down\n");

    return 0;
}