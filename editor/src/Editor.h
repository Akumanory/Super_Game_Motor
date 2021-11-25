#pragma once

#include <Motor/Application/Application.h>

using namespace motor;

class Editor : public Application {
public:
    Editor();
    ~Editor();

    virtual void onInit();
    virtual void onQuit();
    virtual void onUpdate();
};