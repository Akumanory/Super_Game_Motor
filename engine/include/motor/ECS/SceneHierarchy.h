#pragma once 

#include <motor/ECS/Entity.h>
#include <motor/other/ModelLoader.h>

class SceneHierarchy 
{
public:
    SceneHierarchy() = default;
    SceneHierarchy(Scene* scene);

    void SetContext(Scene* scene);
    void OnImguiRender();

    Entity GetSelectedEntity() const { return m_selection_context; }
    void SetSelectedEntity(Entity entity);

private:
    void DrawEntityNode(Entity entity);
    void DrawEntitySubNode(Entity entity, Entity& deliting_entity, bool& deleting_flag);
    void DrawSelectedEntityComponents(Entity entity);

private:
    Scene* m_context;
    Entity m_selection_context;

    std::vector<Condition> productions;
    std::vector<Condition> functions;

    string attribute = "Location";
    std::vector<std::string> signs = { "!", ">", "<", "" };
    std::string selectedSign = "";
    string value = "0";
    bool selfTag = false;
};