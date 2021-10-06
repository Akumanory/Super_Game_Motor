#pragma once

#include <motor/graphics/Camera.h>

class CameraContainer {
public:
    void ImGUIWindow();
    void AddCamera(Camera camera);
    Camera& GetCurrentCamera();

private:
    std::vector<Camera> cameras;
    int _selectedCameraId = 0;
};