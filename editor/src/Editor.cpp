#include "EditorApp.h"

#include <Motor/Engine/Scene.h>
#include <Motor/Engine/Entity.h>
#include <Motor/Component/TransformComponent.h>

Editor::Editor() {
}

Editor::~Editor() {
}

void Editor::onInit() {
    auto window = getEngine()->createWindow();
    auto scene = getEngine()->createScene();

    auto entity = scene->createEntity();
    auto transform = entity->addComponent<ground::GTransformComponent>();
    transform->setPosition(ground::GVec3(100, 0, 100));

    auto entity2 = scene->createEntity();
    auto transform2 = entity2->addComponent<ground::GTransformComponent>();
    transform2->setPosition(ground::GVec3(300, 0, 300));
}

void Editor::onQuit() {
}

void Editor::onUpdate() {
}
