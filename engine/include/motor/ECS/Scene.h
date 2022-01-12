#pragma once

#include <motor/other/ModelLoader.h>
#include <entt/entt.hpp>

#include <filesystem>


//struct RenderableEntities
//{
//    TransformComponent& transform_comp;
//    MeshComponent& mesh_comp;
//
//    //RenderableEntities() = default;
//};

class Entity;

class Scene
{
public:
    Scene() = default;
    Scene(int& height, int& width, ModelLoader* model_manager);
    ~Scene() = default;

    void SetModelLoader(ModelLoader* model_manager);
    void SetAspectRatioParams(int height, int width);
    Entity CreateEntity(const std::string name);
    void DestroyEntity(Entity entity);
    std::vector<Entity> GetRenderableEntities();
    std::vector<Entity> GetPhysicsEntities();
    void Reset();
    Entity GetPrimaryCamera();

    template<typename T>
    std::vector<Entity> GetEntitysByComponent() 
    {
        std::vector<Entity> entities;

        auto view = m_registry.view<T>();

        for (entt::entity entity : view) {
            Entity tmp = { entity, this };
            entities.emplace_back(tmp);
        }
        return entities;
    }

    template <typename T, typename T2>
    std::vector<Entity> GetEntitysByTwoComponents() 
    {
        std::vector<Entity> entities;

        auto view = m_registry.view<T,T2>();

        for (entt::entity entity : view) {
            Entity tmp = { entity, this };
            entities.emplace_back(tmp);
        }
        return entities;
    }

    //std::vector<Entity> GetRenderableEntities();
    //std::vector<Entity> GetCamerasEntities();
    //std::vector<Entity> GetPointLights();
    //std::vector<Entity> GetTransformEntities();

    void OnRednerUpdate();

    void Save(std::filesystem::path);
    void Load(std::filesystem::path);

private:
    template <typename T>
    void OnComponentAdded(Entity entity, T& component);

private:
    int m_window_height;
    int m_window_width;

    entt::registry m_registry;
    ModelLoader* m_model_manager;

    friend class Entity;
    friend class SceneHierarchy;
};