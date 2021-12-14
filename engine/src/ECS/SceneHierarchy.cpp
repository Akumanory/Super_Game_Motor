#include <motor/ECS/SceneHierarchy.h>
#include <motor/ECS/Components.h>
#include <motor/ECS/ComponentSystems.h>

#include "imgui.h"
#include "imgui_internal.h"


SceneHierarchy::SceneHierarchy(Scene* scene) 
{
    SetContext(scene);
}

void SceneHierarchy::SetContext(Scene* scene) 
{
    m_context = scene;
    m_selection_context = {};
}

void SceneHierarchy::OnImguiRender() 
{
    ImGui::Begin("Scene Hierarchy");

    m_context->m_registry.each([&](auto entityID) {
        Entity entity{ entityID, m_context };
        DrawEntityNode(entity);
    });

    ImGui::End();

    ImGui::Begin("Properties");
    if (m_selection_context) 
    {
        DrawSelectedEntityComponents(m_selection_context);
    }
    ImGui::End();
}

void SceneHierarchy::DrawEntityNode(Entity entity) {
    auto& tag = entity.GetComponent<TagComponent>().tag;

    ImGuiTreeNodeFlags flags = ((m_selection_context == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
    if (ImGui::IsItemClicked()) {
        m_selection_context = entity;
    }

    if (opened) {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
        if (opened)
            ImGui::TreePop();
        ImGui::TreePop();
    }
}

void SceneHierarchy::DrawSelectedEntityComponents(Entity entity) {
    if (entity.HasComponent<TagComponent>()) 
    {
        auto& tag = entity.GetComponent<TagComponent>().tag;

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), tag.c_str());
        if (ImGui::InputText("Tag", buffer, sizeof(buffer))) 
        {
            tag = std::string(buffer);
        }
    }

    if (entity.HasComponent<TransformComponent>()) 
    {
        auto& transform_comp = entity.GetComponent<TransformComponent>();

        if (ImGui::DragFloat3("Position", &transform_comp.position.x, 0.01f, -100.0f, 100.0f)) 
        {
            ComponentSystems::UpdateBoundingBox(entity);
        }

        if (ImGui::DragFloat3("Rotation", &transform_comp.rotation.x, 0.01f, -100.0f, 100.0f)) 
        {
            ComponentSystems::UpdateBoundingBox(entity);
        }

        if (ImGui::DragFloat3("Scale", &transform_comp.scale.x, 0.01f, 0.0001f, 100.0f)) 
        {
            ComponentSystems::UpdateBoundingBox(entity);
        }
    }
}