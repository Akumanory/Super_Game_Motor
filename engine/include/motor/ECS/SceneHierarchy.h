#pragma once 

#include <motor/ECS/Entity.h>
#include <motor/other/ModelLoader.h>
#include <motor/other/Physics.hpp>

class SceneHierarchy 
{
public:
    SceneHierarchy() = default;
    SceneHierarchy(Scene* scene);

    void SetContext(Scene* scene);
    void OnImguiRender();

    Entity GetSelectedEntity() const { return m_selection_context; }
    void SetSelectedEntity(Entity entity);

    void SetPhysics(motor::Physics* ph);

private:
    void DrawEntityNode(Entity entity);
    void DrawEntitySubNode(Entity entity, Entity& deliting_entity, bool& deleting_flag);
    void DrawSelectedEntityComponents(Entity entity);

private:
    Scene* m_context;
    Entity m_selection_context;
    motor::Physics* physics_{ nullptr };
};