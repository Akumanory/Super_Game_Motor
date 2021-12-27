#include <motor/ECS/Scene.h>
#include <motor/ECS/Entity.h>
#include <motor/graphics/CameraContainer.h>
#include <motor/ECS/ComponentSystems.h>

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

void Scene::SetAspectRatioParams(int& height, int& width) 
{
    m_window_height = height;
    m_window_width = width;
}

std::vector<Entity> Scene::GetRenderableEntities() {
    std::vector<Entity> renderable_e;

    auto view = m_registry.view<TransformComponent, MeshComponent>();

    for (entt::entity entity : view) 
    {
        Entity tmp = { entity, this };
        renderable_e.emplace_back(tmp);
    }
    return renderable_e;
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
        auto childs = entity.GetComponent<ChildsComponent>().child_entities;
        for (auto&& i : childs) 
        {
            i.RemoveComponent<ParentComponent>();
        }
    }
    if (entity.HasComponent<ParentComponent>()) 
    {
        // TODO: Что то потом с этим придумать а то выглядит ужасно
        auto childs = entity.GetComponent<ParentComponent>().parent->GetComponent<ChildsComponent>().child_entities;
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
            entity.GetComponent<ParentComponent>().parent->RemoveComponent<ChildsComponent>();
        } 
        else 
        {
            entity.GetComponent<ParentComponent>().parent->RemoveComponent<ChildsComponent>();
            entity.GetComponent<ParentComponent>().parent->AddComponent<ChildsComponent>().child_entities = childs;
        }
        
    }

    m_registry.destroy((entt::entity)entity);
}


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
void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component) 
{
    ComponentSystems::SetModel(entity, m_model_manager->GetModelById(0));
}

template <>
void Scene::OnComponentAdded<ParentComponent>(Entity entity, ParentComponent& component) {
}

template <>
void Scene::OnComponentAdded<ChildsComponent>(Entity entity, ChildsComponent& component) {
}
