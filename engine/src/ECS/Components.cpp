#include <motor/ECS/Components.h>
#include <motor/ECS/Entity.h>
#include <entt/entt.hpp>

Scene* CurrentScene = nullptr;

auto TagComponent::from_json(rj::Value& obj) -> TagComponent {
    return TagComponent{ .tag = obj["tag"].GetString() };
}
auto TagComponent::to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void {
    rj::Value tag_j;
    tag_j.SetString(tag.c_str(), tag.length(), rjAllocator);
    obj.AddMember("tag", tag_j, rjAllocator);
}

auto TransformComponent::from_json(rj::Value& obj) -> TransformComponent {
    auto& world_position_j = obj["world_position"];
    auto& world_rotation_j = obj["world_rotation"];
    auto& local_position_j = obj["local_position"];
    auto& local_rotation_j = obj["local_rotation"];
    auto& local_scale_j = obj["local_scale"];
    return TransformComponent{
        .world_position = { world_position_j[0].GetFloat(),
          world_position_j[1].GetFloat(),
          world_position_j[2].GetFloat() },
        .world_rotation = { world_rotation_j[0].GetFloat(),
          world_rotation_j[1].GetFloat(),
          world_rotation_j[2].GetFloat() },
        .local_position = { local_position_j[0].GetFloat(),
          local_position_j[1].GetFloat(),
          local_position_j[2].GetFloat() },
        .local_rotation = { local_rotation_j[0].GetFloat(),
          local_rotation_j[1].GetFloat(),
          local_rotation_j[2].GetFloat() },
        .local_scale = { local_scale_j[0].GetFloat(),
          local_scale_j[1].GetFloat(),
          local_scale_j[2].GetFloat() },
    };
}
auto TransformComponent::to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void {
    rj::Value world_position_j(rj::kArrayType);
    rj::Value world_rotation_j(rj::kArrayType);
    rj::Value local_position_j(rj::kArrayType);
    rj::Value local_rotation_j(rj::kArrayType);
    rj::Value local_scale_j(rj::kArrayType);
    world_position_j.PushBack(rj::Value{ world_position.x }, rjAllocator);
    world_position_j.PushBack(rj::Value{ world_position.y }, rjAllocator);
    world_position_j.PushBack(rj::Value{ world_position.z }, rjAllocator);
    world_rotation_j.PushBack(rj::Value{ world_rotation.x }, rjAllocator);
    world_rotation_j.PushBack(rj::Value{ world_rotation.y }, rjAllocator);
    world_rotation_j.PushBack(rj::Value{ world_rotation.z }, rjAllocator);
    local_position_j.PushBack(rj::Value{ local_position.x }, rjAllocator);
    local_position_j.PushBack(rj::Value{ local_position.y }, rjAllocator);
    local_position_j.PushBack(rj::Value{ local_position.z }, rjAllocator);
    local_rotation_j.PushBack(rj::Value{ local_rotation.x }, rjAllocator);
    local_rotation_j.PushBack(rj::Value{ local_rotation.y }, rjAllocator);
    local_rotation_j.PushBack(rj::Value{ local_rotation.z }, rjAllocator);
    local_scale_j.PushBack(rj::Value{ local_scale.x }, rjAllocator);
    local_scale_j.PushBack(rj::Value{ local_scale.y }, rjAllocator);
    local_scale_j.PushBack(rj::Value{ local_scale.z }, rjAllocator);
    obj.AddMember("world_position", world_position_j, rjAllocator);
    obj.AddMember("world_rotation", world_rotation_j, rjAllocator);
    obj.AddMember("local_position", local_position_j, rjAllocator);
    obj.AddMember("local_rotation", local_rotation_j, rjAllocator);
    obj.AddMember("local_scale", local_scale_j, rjAllocator);
}

ModelLoader* CurrentModelLoader = nullptr;

auto MeshComponent::from_json(rj::Value& obj) -> MeshComponent {
    std::string model_name{ obj["model_name"].GetString() };
    std::string file_path{ obj["file_path"].GetString() };
    auto loaded_model = CurrentModelLoader->GetModelByFilePath(file_path);
    if (!loaded_model) {
        CurrentModelLoader->LoadModel(file_path, model_name);
        loaded_model = CurrentModelLoader->LastLoadedModel();
    }
    return MeshComponent{
        .transformed_bounding_box = {},
        .model = {
          .meshes = { loaded_model->meshes },
          .bounding_box = { loaded_model->bounding_box },
          .model_name = { loaded_model->model_name },
          .file_path = { loaded_model->file_path } }
    };
}
auto MeshComponent::to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void {
    rj::Value model_name_j;
    rj::Value file_path_j;
    model_name_j.SetString(model.model_name.c_str(), model.model_name.length(), rjAllocator);
    file_path_j.SetString(model.file_path.c_str(), model.file_path.length(), rjAllocator);
    obj.AddMember("model_name", model_name_j, rjAllocator);
    obj.AddMember("file_path", file_path_j, rjAllocator);
}

auto ChildsComponent::from_json(rj::Value& obj) -> ChildsComponent {
    auto& child_entities_j = obj["child_entities"];
    ChildsComponent comp;
    comp.child_entities.reserve(child_entities_j.Size());
    for (auto& v : child_entities_j.GetArray()) {
        comp.child_entities.emplace_back(v.IsNull() ? entt::null : (entt::entity)(v.GetUint()), CurrentScene);
    }
    return comp;
}
auto ChildsComponent::to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void {
    rj::Value child_entities_j(rj::kArrayType);
    for (auto& i : child_entities) {
        if (i) {
            child_entities_j.PushBack(rj::Value((std::uint32_t)(i)), rjAllocator);
        } else {
            child_entities_j.PushBack(rj::Value(rj::kNullType), rjAllocator);
        }
    }
    obj.AddMember("child_entities", child_entities_j, rjAllocator);
}

auto ParentComponent::from_json(rj::Value& obj) -> ParentComponent {
    auto& parent_j = obj["parent"];
    return ParentComponent{
        .parent = { parent_j.IsNull() ? entt::null : (entt::entity)(parent_j.GetUint()), CurrentScene }
    };
}
auto ParentComponent::to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void {
    rj::Value parent_j(rj::kNullType);
    if (parent) {
        parent_j.SetUint((std::uint32_t)(parent));
    }
    obj.AddMember("parent", parent_j, rjAllocator);
}

void DialogComponent::SetText(int textNumber) {
    currentDialog = dialogs.at(textNumber);
}

void DialogComponent::AddButton(int buttonNumber) {
    currentAnswers.push_back(buttonNumber);
}

void DialogComponent::ClearButtons() {
    currentAnswers.clear();
}
