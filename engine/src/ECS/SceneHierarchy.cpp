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
        if (!entity.HasComponent<ParentComponent>()) {
            DrawEntityNode(entity);
        }
        //DrawEntityNode(entity);
    });

    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        m_selection_context = {};

    if (ImGui::BeginPopupContextWindow(0, 1, false))
    {
        if (ImGui::MenuItem("Create Empty Entity")) 
        {
            m_context->CreateEntity("Empty Entity");
        }
        ImGui::EndPopup();
    }

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
    if (ImGui::IsItemClicked()) 
    {
        m_selection_context = entity;
    }

    bool entity_deleted = false;
    Entity deliting_entity;
    if (ImGui::BeginPopupContextItem()) 
    {
        if (ImGui::MenuItem("Delete Entity")) 
        {
            entity_deleted = true;
            deliting_entity = entity;
        }
        ImGui::EndPopup();
    }

    if (opened) 
    {
        if (entity.HasComponent<ChildsComponent>()) 
        {
            DrawEntitySubNode(entity, deliting_entity, entity_deleted);
        }
        ImGui::TreePop();
    } 

    if (entity_deleted) 
    {
        m_context->DestroyEntity(deliting_entity);
        if (m_selection_context == deliting_entity) 
        {
            m_selection_context = {};
        }
    }
}

void SceneHierarchy::DrawEntitySubNode(Entity entity, Entity& deliting_entity, bool& deleting_flag) {
    auto& childs_comp = entity.GetComponent<ChildsComponent>();

    for (auto i : childs_comp.child_entities) {
        auto& tag = i.GetComponent<TagComponent>().tag;
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)i, flags, tag.c_str());
        if (ImGui::IsItemClicked()) {
            m_selection_context = i;
        }

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Delete Entity")) {
                deliting_entity = i;
                deleting_flag = true;
            }
            ImGui::EndPopup();
        }

        if (opened) {
            if (i.HasComponent<ChildsComponent>()) {
                DrawEntitySubNode(i, deliting_entity, deleting_flag);
            }
            ImGui::TreePop();
        }
    }
}

static void DrawVec3Control(const std::string& label, DirectX::XMFLOAT3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize))
        values.x = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize))
        values.z = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();
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
        if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_OpenOnArrow, "Transform")) 
        {
            auto& transform_comp = entity.GetComponent<TransformComponent>();

            DrawVec3Control("Position", transform_comp.position);

            DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(
              DirectX::XMConvertToDegrees(transform_comp.rotation.x),
              DirectX::XMConvertToDegrees(transform_comp.rotation.y),
              DirectX::XMConvertToDegrees(transform_comp.rotation.z));

            DrawVec3Control("Rotation", rotation);

            transform_comp.rotation.x = DirectX::XMConvertToRadians(rotation.x);
            transform_comp.rotation.y = DirectX::XMConvertToRadians(rotation.y);
            transform_comp.rotation.z = DirectX::XMConvertToRadians(rotation.z);

            DrawVec3Control("Scale", transform_comp.scale, 1.0f);

            ImGui::TreePop();
        }
    }
}

