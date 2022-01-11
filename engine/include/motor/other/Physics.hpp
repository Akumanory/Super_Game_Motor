#pragma once

#include <btBulletDynamicsCommon.h>

#include <memory>
#include <map>

namespace motor {

class Physics {
public:
    Physics() = default;
    Physics(Physics const&) = delete;
    Physics(Physics&&) = default;
    Physics& operator=(Physics const&) = delete;
    Physics& operator=(Physics&&) = default;
    ~Physics();

    auto Init() -> void;
    auto Clean() -> void;
    auto Update(float delta) -> void;
    auto AddCube(btVector3 location, btVector3 rotation, btVector3 extent, float mass) -> int;
    auto Get(int, btVector3& location, btVector3& rotation) -> void;
    auto SetLocRot(int, btVector3 location, btVector3 rotation) -> void;
    //auto SetExtent(int, btVector3 extent) -> void;
    auto Remove(int) -> void;

private:
    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfig_;
    std::unique_ptr<btCollisionDispatcher> collisionDispatcher_;
    std::unique_ptr<btBroadphaseInterface> overlappingPairCache_;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver_;
    std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld_;
    btAlignedObjectArray<btCollisionShape*> collisionShapes_;
    int currentId_{ 0 };
    std::map<int, btRigidBody*> bodies_;
};

} // namespace motor