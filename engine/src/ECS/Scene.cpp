#include <motor/ECS/Scene.h>
#include <motor/ECS/Archive.hpp>
#include <motor/graphics/CameraContainer.h>
#include <motor/ECS/ComponentSystems.h>

#include <fstream>

Scene::Scene() 
{

}

Scene::Scene(int& height, int& width, ModelLoader* model_manager) 
{
    SetModelLoader(model_manager);
    SetAspectRatioParams(height, width);
}

Scene::~Scene() 
{
}

void Scene::SetModelLoader(ModelLoader* model_manager) 
{
    m_model_manager = model_manager;
}

void Scene::SetAspectRatioParams(int height, int width) 
{
    m_window_height = height;
    m_window_width = width;
}

#pragma region OldRealization
//std::vector<Entity> Scene::GetRenderableEntities() {
//    std::vector<Entity> renderable_e;
//
//    auto view = m_registry.view<TransformComponent, MeshComponent>();
//
//    for (entt::entity entity : view) 
//    {
//        Entity tmp = { entity, this };
//        renderable_e.emplace_back(tmp);
//    }
//    return renderable_e;
//}
//
//std::vector<Entity> Scene::GetCamerasEntities() 
//{
//    std::vector<Entity> cameras_e;
//
//    auto view = m_registry.view<TransformComponent, CameraComponent>();
//
//    for (entt::entity entity : view) {
//        Entity tmp = { entity, this };
//        cameras_e.emplace_back(tmp);
//    }
//    return cameras_e;
//}
//
//std::vector<Entity> Scene::GetPointLights() 
//{
//    std::vector<Entity> point_light_e;
//
//    auto view = m_registry.view<TransformComponent, PointLightComponent>();
//
//    for (entt::entity entity : view) {
//        Entity tmp = { entity, this };
//        point_light_e.emplace_back(tmp);
//    }
//    return point_light_e;
//}
//
//std::vector<Entity> Scene::GetTransformEntities() 
//{
//    std::vector<Entity> transform_e;
//
//    auto view = m_registry.view<TransformComponent>();
//
//    for (entt::entity entity : view) {
//        Entity tmp = { entity, this };
//        transform_e.emplace_back(tmp);
//    }
//    return transform_e;
//}
#pragma endregion

Entity Scene::GetPrimaryCamera() 
{
    auto view = m_registry.view<CameraComponent>();
    for (auto entity : view) {
        const auto& camera = view.get<CameraComponent>(entity);
        if (camera.primary)
            return Entity{ entity, this };
    }
    return {};
}

void Scene::OnRednerUpdate() 
{
    auto view = m_registry.view<TransformComponent>();
    for (auto entity : view) {
        Entity temp = Entity{ entity, this };
        ComponentSystems::UpdateBoundingBox(temp);
        ComponentSystems::UpdateBoundingFrustum(temp);
    }
}

Entity Scene::CreateEntity(const std::string name) 
{
    Entity entity = { m_registry.create(), this };
    entity.AddComponent<TransformComponent>();
    auto& l_tag = entity.AddComponent<TagComponent>();
    l_tag.tag = name.empty() ? "Empty_name" : name;
    return entity;
}

void Scene::DestroyEntity(Entity entity) 
{
    // избавится от зависимостей перед удалением
    if (entity.HasComponent<ChildsComponent>()) 
    {
        auto& childs = entity.GetComponent<ChildsComponent>().child_entities;
        for (auto& i : childs) 
        {
            i.RemoveComponent<ParentComponent>();
        }
    }
    if (entity.HasComponent<ParentComponent>()) 
    {
        // TODO: Что то потом с этим придумать а то выглядит ужасно
        auto& childs = entity.GetComponent<ParentComponent>().parent.GetComponent<ChildsComponent>().child_entities;
        for (size_t i = 0; i < childs.size(); i++) 
        {
            if (childs[i] == entity) 
            {
                childs.erase(childs.begin() + i);
                break;
            }
        }
        if (childs.size() == 0) 
        {
            entity.GetComponent<ParentComponent>().parent.RemoveComponent<ChildsComponent>();
        } 
        else 
        {
            entity.GetComponent<ParentComponent>().parent.RemoveComponent<ChildsComponent>();
            entity.GetComponent<ParentComponent>().parent.AddComponent<ChildsComponent>().child_entities = childs;
        }
        
    }

    m_registry.destroy((entt::entity)entity);
}

#pragma region OnCompAdeded
template <typename T>
void Scene::OnComponentAdded(Entity entity, T& component) {
    static_assert(false);
}

template <>
void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {
}

template <>
void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {
}

template <>
void Scene::OnComponentAdded<PointLightComponent>(Entity entity, PointLightComponent& component) {
}

template <>
void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
    if (m_window_width > 0 && m_window_height > 0)
        component.camera.SetViewPortSize(m_window_height, m_window_width);
}

template <>
void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component) {
    ComponentSystems::SetModel(entity, m_model_manager->GetModelById(0));
}

template <>
void Scene::OnComponentAdded<ParentComponent>(Entity entity, ParentComponent& component) {
    if (!component.parent.HasComponent<ChildsComponent>()) {
        component.parent.AddComponent<ChildsComponent>();
    }
    auto& childs_comp = component.parent.GetComponent<ChildsComponent>();
    childs_comp.child_entities.emplace_back(entity);
}

template <>
void Scene::OnComponentAdded<ChildsComponent>(Entity entity, ChildsComponent& component) {
}
#pragma endregion


const char* FileName = "scene.json";

void Scene::Save() {
    RapidJsonOutputArchive json_archive;
    entt::basic_snapshot snapshot(m_registry);
    snapshot.entities(json_archive)
      .component<
        TagComponent,
        TransformComponent,
        MeshComponent,
        ChildsComponent,
        ParentComponent>(json_archive);
    json_archive.Close();
    std::string json_output = json_archive.AsString();
    std::ofstream file_out(FileName);
    file_out << json_output;
}

extern Scene* CurrentScene;
extern ModelLoader* CurrentModelLoader;

void Scene::Load() {
    CurrentScene = this;
    CurrentModelLoader = m_model_manager;
    std::ifstream file_in(FileName);
    //RapidJsonInputArchive json_in(file_in);

    //std::stringstream json_input;
    //json_input << file_in.rdbuf();
    //RapidJsonInputArchive json_in(json_input.str());

    std::string json_input;
    std::string s;
    while (std::getline(file_in, s)) {
        json_input += s;
    }
    RapidJsonInputArchive json_in(json_input);

    //entt::registry reg2;
    //entt::basic_snapshot_loader loader(reg2);
    //loader.entities(json_in)
    //  .component<TagComponent, TransformComponent, ChildsComponent>(json_in);
    entt::registry reg;
    entt::snapshot_loader{ reg }
      .entities(json_in)
      .component<
        TagComponent,
        TransformComponent,
        MeshComponent,
        ChildsComponent,
        ParentComponent>(json_in)
      .orphans();
    //auto m_view = m_registry.view<MeshComponent>();
    //m_view.each([&reg](const auto entity, auto& mesh) {
    //    reg.emplace<MeshComponent>(entity, std::move(mesh));
    //});
    std::swap(m_registry, reg);
}