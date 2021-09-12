#include <motor/utils.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <format>
#include <iostream>

using namespace std;
using namespace motor;

int WINAPI WinMain(
  _In_ HINSTANCE h_instance,
  _In_opt_ HINSTANCE h_prev_instance,
  _In_ LPSTR lp_cmd_line,
  _In_ int n_show_cmd) {

#if _DEBUG
    utils::enable_console();
#endif // _DEBUG

    try {
        throw_if_failed(E_FAIL);
    } catch (com_exception& e) {
        utils::debug_write::error(e.what());
    }

    Sleep(5000);

    return 0;
}