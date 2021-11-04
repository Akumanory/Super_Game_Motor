#include <motor/ui_system/ui_console.hpp>

using namespace motor;
using namespace ui_system;

ConsoleUI::ConsoleUI() {
    memset(input_buf_, 0, sizeof(input_buf_));
}

void ConsoleUI::Draw(const char* title, bool* p_open) {
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }
    if (core_system::LuaConsole::getInstance() == nullptr) {
        auto color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextWrapped("Console is not created");
        ImGui::PopStyleColor();
        ImGui::End();
        return;
    }

    auto&& log = core_system::LuaConsole::getInstance()->getLog();

    // Options menu
    if (ImGui::BeginPopup("Options")) {
        ImGui::Checkbox("Auto-scroll", &auto_scroll_);
        ImGui::EndPopup();
    }

    // Options, Filter
    if (ImGui::Button("Options")) {
        ImGui::OpenPopup("Options");
    }
    ImGui::Separator();

    // Reserve enough left-over height for 1 separator + 1 input text
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::Selectable("Clear"))
            //ClearLog();
            ImGui::EndPopup();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

    for (std::string_view line : log) {
        ImVec4 color;
        bool has_color{ false };
        if (strstr(line.data(), "[error]")) {
            color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
            has_color = true;
        } else if (strncmp(line.data(), "# ", 2) == 0) {
            color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
            has_color = true;
        }
        if (has_color) {
            ImGui::PushStyleColor(ImGuiCol_Text, color);
        }
        ImGui::TextUnformatted(line.data());
        if (has_color) {
            ImGui::PopStyleColor();
        }
    }
    //for (int i = 0; i < log.size(); i++) {
    //    const char* item = Items[i];
    //    if (!Filter.PassFilter(item))
    //        continue;
    //
    //    // Normally you would store more information in your item than just a string.
    //    // (e.g. make Items[] an array of structure, store color/type etc.)
    //    ImVec4 color;
    //    bool has_color = false;
    //    if (strstr(item, "[error]")) {
    //        color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
    //        has_color = true;
    //    } else if (strncmp(item, "# ", 2) == 0) {
    //        color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
    //        has_color = true;
    //    }
    //    if (has_color)
    //        ImGui::PushStyleColor(ImGuiCol_Text, color);
    //    ImGui::TextUnformatted(item);
    //    if (has_color)
    //        ImGui::PopStyleColor();
    //}

    if (scroll_to_bottom_ || (auto_scroll_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) {
        ImGui::SetScrollHereY(1.0f);
    }
    scroll_to_bottom_ = false;

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::Separator();

    // Command-line
    bool reclaim_focus = false;
    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
    if (ImGui::InputText(
          "Input",
          input_buf_,
          IM_ARRAYSIZE(input_buf_),
          input_text_flags,
          [](auto* data) { return ((ConsoleUI*)data->UserData)->TextEditCallback(data); },
          (void*)this)) {
        char* s = input_buf_;
        Strtrim(s);
        if (s[0])
            ExecCommand(s);
        strcpy(s, "");
        reclaim_focus = true;
    }

    // Auto-focus on window apparition
    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

    ImGui::End();
}

// Portable helpers
int ConsoleUI::Stricmp(const char* s1, const char* s2) {
    int d;
    while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
        s1++;
        s2++;
    }
    return d;
}

int ConsoleUI::Strnicmp(const char* s1, const char* s2, int n) {
    int d = 0;
    while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
        s1++;
        s2++;
        n--;
    }
    return d;
}

char* ConsoleUI::Strdup(const char* s) {
    IM_ASSERT(s);
    size_t len = strlen(s) + 1;
    void* buf = malloc(len);
    IM_ASSERT(buf);
    return (char*)memcpy(buf, (const void*)s, len);
}

void ConsoleUI::Strtrim(char* s) {
    char* str_end = s + strlen(s);
    while (str_end > s && str_end[-1] == ' ')
        str_end--;
    *str_end = 0;
}

int ConsoleUI::TextEditCallback(ImGuiInputTextCallbackData* data) {
    switch (data->EventFlag) {
    case ImGuiInputTextFlags_CallbackCompletion: {
        break;
    }
    case ImGuiInputTextFlags_CallbackHistory: {
    }
    }
    return 0;
}

void ConsoleUI::ExecCommand(const char* command_line) {
    core_system::LuaConsole::getInstance()->input(command_line);

    // On command input, we scroll to bottom even if AutoScroll==false
    scroll_to_bottom_ = true;
}
