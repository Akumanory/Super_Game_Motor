#pragma once

#include <motor/ui_system/ui.hpp>
#include <motor/core_system/console.hpp>

namespace motor {
namespace ui_system {

class ConsoleUI : public UI {
public:
    ConsoleUI();
    ConsoleUI(ConsoleUI const&) = delete;
    ConsoleUI(ConsoleUI&&) = delete;
    ConsoleUI& operator=(ConsoleUI const&) = delete;
    ConsoleUI& operator=(ConsoleUI&&) = delete;
    ~ConsoleUI() override = default;

public:
    void Draw(const char* title, bool* p_open) override;

private:
    // Portable helpers
    static int Stricmp(const char* s1, const char* s2);
    static int Strnicmp(const char* s1, const char* s2, int n);
    static char* Strdup(const char* s);
    static void Strtrim(char* s);

    int TextEditCallback(ImGuiInputTextCallbackData* data);
    void ExecCommand(const char* command_line);

private:
    char input_buf_[1024];
    bool auto_scroll_{ true };
    bool scroll_to_bottom_{ false };
    bool continuation_{ false };
};

} // namespace ui_system
} // namespace motor