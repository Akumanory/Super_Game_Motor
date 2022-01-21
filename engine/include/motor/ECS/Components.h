#pragma once

#include <motor/ECS/Entity.h>
#include <motor/graphics/UpdatedModel.h>
#include <motor/graphics/MeshForComponents.h>
#include <motor/other/ModelLoader.h>
#include <motor/graphics/SceneCamera.h>
#undef max
#include <rapidjson/document.h>

using namespace DirectX;

namespace rj = rapidjson;

// Component Done
struct TagComponent {
    std::string tag = "Undefined Tag";

    static auto from_json(rj::Value& obj) -> TagComponent;
    auto to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void;
};

// Component Done
struct TransformComponent {
    XMFLOAT3 world_position = { 0.0, 0.0, 0.0 };
    XMFLOAT3 world_rotation = { 0.0, 0.0, 0.0 };

    XMFLOAT3 local_position = { 0.0, 0.0, 0.0 };
    XMFLOAT3 local_rotation = { 0.0, 0.0, 0.0 };
    XMFLOAT3 local_scale    = { 1.0, 1.0, 1.0 };


    XMVECTOR GetUpVector() 
    {
        return XMVector3TransformCoord(
          DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
          XMMatrixRotationRollPitchYaw(world_rotation.x, world_rotation.y, world_rotation.z));
    }

    XMVECTOR GetForwardVector() 
    {
        return XMVector3TransformCoord(
            DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), 
            XMMatrixRotationRollPitchYaw(world_rotation.x, world_rotation.y, world_rotation.z));
    }

    XMVECTOR GetRightVector() 
    {
        return XMVector3TransformCoord(
          DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f),
          XMMatrixRotationRollPitchYaw(world_rotation.x, world_rotation.y, world_rotation.z));
    }

    // translation * rotation * scale
    XMMATRIX GetWorldTransformMatrix() const {
        return XMMatrixRotationRollPitchYaw(world_rotation.x, world_rotation.y, world_rotation.z) * XMMatrixTranslation(world_position.x, world_position.y, world_position.z);
    }

    XMMATRIX GetLocalTransformMatrix() const {
        return XMMatrixScaling(local_scale.x, local_scale.y, local_scale.z) * XMMatrixRotationRollPitchYaw(local_rotation.x, local_rotation.y, local_rotation.z) * XMMatrixTranslation(local_position.x, local_position.y, local_position.z) * GetWorldTransformMatrix();
    }

    static auto from_json(rj::Value& obj) -> TransformComponent;
    auto to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void;
};

struct Net {

    ReteNodePtr RootNode;
    TestAtTokenFilterNode testAtTokenFilterNode;
    std::unordered_map<Condition, AlphaMemoryPtr> conditionToAlphaMemory;
    std::unordered_map<StructForHash, ReteNodePtr> dict;
    std::unordered_set<ProductionNodePtr> resultNodes;

    ParamTestNodeVector GetTestsFromCondition(Condition c,
      const ConditionVector& condsHigherUp);

    ReteNodePtr BuildOrShareJoinNode(ReteNodePtr parent, AlphaMemoryPtr am,
      const ParamTestNodeVector& tests, const Condition& c);

    ReteNodePtr BuildOrShareTokenFilterNode(ReteNodePtr parent, AlphaMemoryPtr am,
      const ParamTestNodeVector& tests, const Condition& c);

    AlphaMemoryPtr BuildOrShareAlphaMemory(const Condition& c);

    ReteNodePtr BuildOrShareNetworkForConditions(ReteNodePtr parent,
      const ConditionVector& conds, ConditionVector condsHigherUp);

    static auto from_json(rj::Value& obj) -> Net;
    auto to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void;

    Net();

    void AddProduction(const ConditionVector& conditions, const std::vector<Condition>& getter);

    std::vector<ConditionVector> invoke();

    void ClearStatus();

    void AddWME(const WME& wme);

    void AddFunction(const std::string& key, TestAtTokenFilterNode::JudgeFunctionType judgeFunction);
};

struct MeshComponent {
    BoundingOrientedBox transformed_bounding_box;
    ModelStruct model;
    static auto from_json(rj::Value& obj) -> MeshComponent;
    auto to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void;
};

// Компонент указывает на то что entity является родлителем для данных entity
struct ChildsComponent {
    std::vector<Entity> child_entities;
    static auto from_json(rj::Value& obj) -> ChildsComponent;
    auto to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void;
};

// Коспонет указывает на то что entity является чьим то ребенком
struct ParentComponent {
    Entity parent;
    static auto from_json(rj::Value& obj) -> ParentComponent;
    auto to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void;
};

struct CameraComponent
{
    SceneCamera camera;
    bool primary = false;
};