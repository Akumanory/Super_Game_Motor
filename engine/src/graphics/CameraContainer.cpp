#include <motor/graphics/CameraContainer.h>

// ImGui
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

void CameraContainer::ImGUIWindow() 
{
    if (ImGui::Begin("Test")) 
    {
        if (ImGui::BeginCombo("Active Camera", cameras[_selectedCameraId].GetCameraName().c_str())) 
        {
            for (size_t i = 0; i < std::size(cameras); i++) 
            {
                const bool isSelected = i == _selectedCameraId;
                if (ImGui::Selectable(cameras[i].GetCameraName().c_str(), isSelected)) 
                {
                    _selectedCameraId = i;
                }
            }
            ImGui::EndCombo();
        }

        //TODO тут можно вызывать виджеты относящиеся к камере
    }
    ImGui::End();
}

Camera& CameraContainer::GetCurrentCamera() 
{
    return cameras[_selectedCameraId];
}

Camera& CameraContainer::GetCameraById(int id) 
{ 
    return cameras[id];
}

void CameraContainer::AddCamera(Camera camera) 
{
    cameras.push_back(std::move(camera));
}

size_t CameraContainer::CountCameras() 
{
    return cameras.size();
}

void CameraContainer::Reset() {
    cameras.clear();
    _selectedCameraId = 0;
}