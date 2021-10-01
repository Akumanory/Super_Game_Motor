#include <motor/utils.hpp>
#include <motor/task_system.hpp>

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

    auto handle = GetCurrentThread();
    SetThreadAffinityMask(handle, 1);

    task_system::timer real_timer;
    task_system::ticker<task_system::ticker_type::CONSTANT> ticker{ real_timer, chrono::milliseconds(200) };

    auto tick_tack = task_system::tickable_object{
        [&real_timer](task_system::delta_time delta) {
            cout << "tick " << real_timer.elapsed() << endl;
        }
    };
    ticker.tickables().emplace_back(&tick_tack);

    while (real_timer.elapsed() < chrono::seconds(30)) {
        ticker.tick();
        this_thread::yield();
    }
    this_thread::sleep_for(chrono::seconds(7));

    return 0;
}
