#include <motor/ECS/SceneHierarchy.h>
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
        if (ImGui::MenuItem("Create Camera Entity")) 
        {
            Entity temp_entt = m_context->CreateEntity("Camera Entity");
            temp_entt.AddComponent<CameraComponent>();
        }
        if (ImGui::MenuItem("Create Entity With Model 3D")) {
            Entity temp_entt = m_context->CreateEntity("Model Entity");
            temp_entt.AddComponent<MeshComponent>();
        }
        if (ImGui::MenuItem("Create Entity With Point Light")) {
            Entity temp_entt = m_context->CreateEntity("Point Light");
            temp_entt.AddComponent<PointLightComponent>();
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

    ImGui::Begin("Serialization");
    if (ImGui::Button("Save")) {
        m_context->Save();
    }
    if (ImGui::Button("Load")) {
        m_context->Load();
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

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        // Set payload to carry the index of our item (could be anything)
        ImGui::SetDragDropPayload("Entity", &entity, sizeof(entity));

        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity")) {
            IM_ASSERT(payload->DataSize == sizeof(entity));
            Entity payload_entity = *(const Entity*)payload->Data;

            payload_entity.AddComponent<ParentComponent>(entity);
        }
        ImGui::EndDragDropTarget();
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

static void DrawVec3Control(const std::string& label, DirectX::XMFLOAT3& values, float resetValue = 0.0f, float columnWidth = 150.0f) {
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


template <typename T, typename LambdaFunction>
static void DrawComponent(const std::string& name, Entity entity, LambdaFunction lambda_function) 
{
    const ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
    if (entity.HasComponent<T>()) {
        auto& component = entity.GetComponent<T>();
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), tree_node_flags, name.c_str());
        ImGui::PopStyleVar();
        
        
        bool removeComponent = false;
        if (open) {
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button("*", ImVec2{ lineHeight, lineHeight })) {
                ImGui::OpenPopup(name.c_str());
            }

            if (ImGui::BeginPopup(name.c_str())) {
                if (ImGui::MenuItem("Remove component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            lambda_function(component);
            ImGui::TreePop();
        }

        if (removeComponent)
            entity.RemoveComponent<T>();
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
        ImGui::NewLine();
    }

    if (entity.HasComponent<TransformComponent>()) 
    {
        if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, "Transform")) 
        {
            auto& transform_comp = entity.GetComponent<TransformComponent>();

            ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4{ 0.1f, 0.8f, 0.5f, 1.0f });
            ImGui::Text("World_Transform");
            ImGui::PopStyleColor();

            DrawVec3Control("WorldPosition", transform_comp.world_position);

            DirectX::XMFLOAT3 w_rotation = DirectX::XMFLOAT3(
              DirectX::XMConvertToDegrees(transform_comp.world_rotation.x),
              DirectX::XMConvertToDegrees(transform_comp.world_rotation.y),
              DirectX::XMConvertToDegrees(transform_comp.world_rotation.z));

            DrawVec3Control("WorldRotation", w_rotation);

            transform_comp.world_rotation.x = DirectX::XMConvertToRadians(w_rotation.x);
            transform_comp.world_rotation.y = DirectX::XMConvertToRadians(w_rotation.y);
            transform_comp.world_rotation.z = DirectX::XMConvertToRadians(w_rotation.z);

            ImGui::NewLine();

            ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4{ 0.1f, 0.8f, 0.5f, 1.0f });
            ImGui::Text("Local_Transform");
            ImGui::PopStyleColor();


            DrawVec3Control("LocalPosition", transform_comp.local_position);

            DirectX::XMFLOAT3 l_rotation = DirectX::XMFLOAT3(
              DirectX::XMConvertToDegrees(transform_comp.local_rotation.x),
              DirectX::XMConvertToDegrees(transform_comp.local_rotation.y),
              DirectX::XMConvertToDegrees(transform_comp.local_rotation.z));

            DrawVec3Control("LocalRotation", l_rotation);

            transform_comp.local_rotation.x = DirectX::XMConvertToRadians(l_rotation.x);
            transform_comp.local_rotation.y = DirectX::XMConvertToRadians(l_rotation.y);
            transform_comp.local_rotation.z = DirectX::XMConvertToRadians(l_rotation.z);

            DrawVec3Control("LocalScale", transform_comp.local_scale, 1.0f);

            ImGui::NewLine();

            ImGui::TreePop();
        }
    }

    DrawComponent<MeshComponent>("Model", entity, [&](auto& component) {
        static int item_current_idx = 0; // Here we store our selection data as an index.
        const char* combo_preview_value = component.model.model_name.c_str(); // Pass in the preview value visible before opening the combo (it could be anything)
        if (ImGui::BeginCombo("Models", combo_preview_value, 0)) {
            for (int n = 0; n < m_context->m_model_manager->_models.size(); n++) {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(m_context->m_model_manager->_models[n]->model_name.c_str(), is_selected))
                    item_current_idx = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();

            ComponentSystems::SetModel(entity, *(m_context->m_model_manager->_models[item_current_idx]));
        }
        ImGui::NewLine();
    });

    DrawComponent<ScriptComponent>("Script", entity, [](auto& component) {
        
        auto& script = component.script;

        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

        char buffer[1024 * 16];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), script.c_str());

        if (ImGui::InputTextMultiline("Script", buffer, IM_ARRAYSIZE(buffer), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags)) 
        {
            script = std::string(buffer);
        }

        ImGui::NewLine();
    });
    
    DrawComponent<PointLightComponent>("PointLight", entity, [](auto& component) {
        ImGui::ColorEdit3("Dynamic Light Color", (float*)&component.lightColor);
        ImGui::DragFloat("Dynamic Light Strength", &component.lightStrength, 0.01f, 0.0f, 100.0f);
        ImGui::DragFloat("Dynamic Light Attenuation A", &component.attennuation_A, 0.01f, 0.1f, 10.0f);
        ImGui::DragFloat("Dynamic Light Attenuation B", &component.attennuation_B, 0.01f, 0.0f, 10.0f);
        ImGui::DragFloat("Dynamic Light Attenuation C", &component.attennuation_C, 0.01f, 0.0f, 10.0f);

        ImGui::NewLine();
    });

    DrawComponent<CameraComponent>("Camera", entity, [](auto& component) 
    {
        ImGui::Checkbox("Primary", &component.primary);

        float fov = component.camera.GetVerticalFOV();
        if (ImGui::DragFloat("FOV", &fov, 1.0f, 1.0f, 179.0f)) {
            component.camera.SetVerticalFOV(fov);
        }

        float nearZ = component.camera.GetNearZ();
        float farZ = component.camera.GetFarZ();
        if (ImGui::DragFloat("NearZ", &nearZ, 1.0f, 0.1f, farZ - 1.0f)) {
            component.camera.SetNearZ(nearZ);
        }
        if (ImGui::DragFloat("FarZ", &farZ, 1.0f, nearZ + 1.0f, 1000.0f)) {
            component.camera.SetFarZ(farZ);
        }
        ImGui::NewLine();
    });
    
    // Child and Parent не DrawComponet потому что особое удаление
    if (entity.HasComponent<ParentComponent>()) 
    {
        auto& component = entity.GetComponent<ParentComponent>();
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        bool open = ImGui::TreeNodeEx((void*)typeid(ParentComponent).hash_code(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, "Parent");
        ImGui::PopStyleVar();
        
        
        bool removeComponent = false;
        if (open) {
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button("*", ImVec2{ lineHeight, lineHeight })) {
                ImGui::OpenPopup("Parent");
            }

            if (ImGui::BeginPopup("Parent")) {
                if (ImGui::MenuItem("Remove component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            ImGui::Text("Parent:");
            ImGui::SameLine();
            ImGui::Text(entity.GetComponent<ParentComponent>().parent.GetComponent<TagComponent>().tag.c_str());

            ImGui::NewLine();

            ImGui::TreePop();
        }

        if (removeComponent) 
        {
            auto& childs = component.parent.GetComponent<ChildsComponent>().child_entities;

            for (size_t i = 0; i < childs.size(); i++) 
            {
                if (childs[i] = entity) 
                {
                    childs.erase(childs.begin() + i);
                    break;
                }
            }

            if (childs.size() == 0) 
            {
                component.parent.RemoveComponent<ChildsComponent>();
            }

            entity.RemoveComponent<ParentComponent>();
        }
    }

    if (entity.HasComponent<ChildsComponent>()) {

        auto& component = entity.GetComponent<ChildsComponent>();
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        bool open = ImGui::TreeNodeEx((void*)typeid(ChildsComponent).hash_code(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, "Childs"); 
        ImGui::PopStyleVar();

        bool removeComponent = false;
        if (open) {
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button("*", ImVec2{ lineHeight, lineHeight })) {
                ImGui::OpenPopup("Childs");
            }

            if (ImGui::BeginPopup("Childs")) {
                if (ImGui::MenuItem("Remove component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            ImGui::Text("Childs:");

            for (auto& i : component.child_entities) {
                ImGui::Text(i.GetComponent<TagComponent>().tag.c_str());
            }

            ImGui::NewLine();

            ImGui::TreePop();
        }

        if (removeComponent) {
            
            auto& childs = component.child_entities;

            for (auto& i : childs) 
            {
                i.RemoveComponent<ParentComponent>();
            }

            entity.RemoveComponent<ChildsComponent>();
        }
    }

    ImGui::NewLine();

    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("AddComponent");
    }

    if (ImGui::BeginPopup("AddComponent")) {
        if (!m_selection_context.HasComponent<MeshComponent>()) {
            if (ImGui::MenuItem("Model")) {
                m_selection_context.AddComponent<MeshComponent>();

                ImGui::CloseCurrentPopup();
            }
        }

        if (!m_selection_context.HasComponent<CameraComponent>()) {
            if (ImGui::MenuItem("Camera")) {
                m_selection_context.AddComponent<CameraComponent>();

                ImGui::CloseCurrentPopup();
            }
        }

        if (!m_selection_context.HasComponent<PointLightComponent>()) {
            if (ImGui::MenuItem("PointLight")) {
                m_selection_context.AddComponent<PointLightComponent>();

                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
}

