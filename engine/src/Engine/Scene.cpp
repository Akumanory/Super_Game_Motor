#include <Motor/Engine/Scene.h>

using namespace motor;

Scene::Scene() : m_ecManager(this) {}

Scene::~Scene() {}

void Scene::release() {
    //ReleaseFn(this);
}

Entity* Scene::createEntity() {
    return m_ecManager.createEntity();
}

//void Scene::addTransformChanged(TransformComponent* transform) {
//    m_setTransformsChanged.emplace(transform);
//}

void Scene::update() {
    for (auto t : m_setTransformsChanged) {
        t->updateMatrix();
    }
    m_setTransformsChanged.clear();
}
