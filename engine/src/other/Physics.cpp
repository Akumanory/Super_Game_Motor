#include <motor/other/Physics.hpp>

using namespace motor;

Physics::~Physics() {
    Clean();
}

auto Physics::Init() -> void {
    collisionConfig_ = std::make_unique<btDefaultCollisionConfiguration>();
    collisionDispatcher_ = std::make_unique<btCollisionDispatcher>(collisionConfig_.get());
    overlappingPairCache_ = std::make_unique<btDbvtBroadphase>();
    solver_ = std::make_unique<btSequentialImpulseConstraintSolver>();
    dynamicsWorld_ = std::make_unique<btDiscreteDynamicsWorld>(
      collisionDispatcher_.get(),
      overlappingPairCache_.get(),
      solver_.get(),
      collisionConfig_.get());

    dynamicsWorld_->setGravity(btVector3(0, -1, 0));
}

auto Physics::Clean() -> void {
    if (not dynamicsWorld_) {
        return;
    }
    for (int i = dynamicsWorld_->getNumCollisionObjects() - 1; i >= 0; i--) {
        btCollisionObject* obj = dynamicsWorld_->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState()) {
            delete body->getMotionState();
        }
        dynamicsWorld_->removeCollisionObject(obj);
        delete obj;
    }

    for (int j = 0; j < collisionShapes_.size(); j++) {
        btCollisionShape* shape = collisionShapes_[j];
        collisionShapes_[j] = 0;
        delete shape;
    }
}

auto Physics::Update(float delta) -> void {
    dynamicsWorld_->stepSimulation(delta, 10);

    //print positions of all objects
    for (int j = dynamicsWorld_->getNumCollisionObjects() - 1; j >= 0; j--) {
        btCollisionObject* obj = dynamicsWorld_->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        if (body && body->getMotionState()) {
            body->getMotionState()->getWorldTransform(trans);
        } else {
            trans = obj->getWorldTransform();
        }
        printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
    }
    int x = 2;
}

auto Physics::AddCube(btVector3 location, btVector3 rotation, btVector3 extent, float mass) -> int {
    btCollisionShape* shape = new btBoxShape(extent);
    collisionShapes_.push_back(shape);

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(location);
    transform.setRotation(btQuaternion{ rotation.x(), rotation.y(), rotation.z() });

    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic) {
        shape->calculateLocalInertia(mass, localInertia);
    }

    btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    dynamicsWorld_->addRigidBody(body);

    bodies_[currentId_] = body;
    return currentId_++;
}

auto Physics::Get(int id, btVector3& location, btVector3& rotation) -> void {
    auto body = bodies_[id];
    //btCollisionObject* obj = dynamicsWorld_->getCollisionObjectArray()[id];
    //btRigidBody* body = btRigidBody::upcast(obj);
    btTransform trans;
    if (body && body->getMotionState()) {
        body->getMotionState()->getWorldTransform(trans);
    } //else {
    //   trans = obj->getWorldTransform();
    //}
    location = trans.getOrigin();
    auto rot = trans.getRotation();
    rot.getEulerZYX(rotation.m_floats[0], rotation.m_floats[1], rotation.m_floats[2]);
}

auto Physics::SetLocRot(int id, btVector3 location, btVector3 rotation) -> void {
    auto body = bodies_[id];
    btTransform trans;
    body->getMotionState()->getWorldTransform(trans);
    trans.setOrigin(location);
    trans.setRotation(btQuaternion{ rotation.x(), rotation.y(), rotation.z() });
    body->getMotionState()->setWorldTransform(trans);
}

//auto Physics::SetExtent(int, btVector3 extent, float mass) -> void {
//
//}

auto Physics::Remove(int id) -> void {
    auto it = bodies_.find(id);
    auto body = it->second;
    bodies_.erase(it);
    dynamicsWorld_->removeRigidBody(body);
    if (body && body->getMotionState()) {
        delete body->getMotionState();
    }
    delete body;
}