#pragma once 

#include <motor/ECS/Entity.h>
#include <motor/ECS/Scene.h>
#include <motor/other/ModelLoader.h>

class SceneHierarchy 
{
public:
    SceneHierarchy() = default;
    SceneHierarchy(Scene* scene, ModelLoader& model_manager);

    void SetContext(Scene* scene, ModelLoader& model_manager);
    void OnImguiRender();

    Entity GetSelectedEntity() const { return m_selection_context; }
    void SetSelectedEntity(Entity entity);

private:
    void DrawEntityNode(Entity entity);
    void DrawEntitySubNode(Entity entity, Entity& deliting_entity, bool& deleting_flag);
    void DrawSelectedEntityComponents(Entity entity);

private:
    Scene* m_context;
    ModelLoader m_model_manager;
    Entity m_selection_context;
};