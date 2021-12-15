﻿#include <motor/ECS/Scene.h>
#include <motor/ECS/Entity.h>
#include <motor/graphics/CameraContainer.h>

Scene::Scene() 
{
}

Scene::~Scene() 
{
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

//void Scene::InitializeSceneEntt(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader) {
//
//    entt::entity first_entity = SimpleGameObject(m_registry);
//
//    SetPosition(m_registry.get<PositionRotation>(first_entity), DirectX::XMFLOAT3(0.0f, 6.0f, 0.0f));
//
//    SetRotation(m_registry.get<PositionRotation>(first_entity), DirectX::XMFLOAT3(0.0f, 2.0f, 0.0f));
//
//    SetModel(m_registry.get<ObjectModel>(first_entity), filePath, device, deviceContext, cb_vs_vertexshader);
//
//    UpdateBoundingBox(m_registry.get<PositionRotation>(first_entity), m_registry.get<ObjectModel>(first_entity));
//
//
//    // New realization
//    entt::entity second_entity = TestNewGameObject(m_registry);
//
//    ComponentSystems::SetPosition(m_registry.get<TransformComponent>(second_entity), DirectX::XMFLOAT3(0.0f, 6.0f, 0.0f));
//    ComponentSystems::SetRotation(m_registry.get<TransformComponent>(second_entity), DirectX::XMFLOAT3(0.5f, 0.0f, 0.0f));
//
//
//    /*
//    entt::entity entity = registry.create();
//    registry.emplace<PositionVector>(entity, DirectX::XMVectorSet(0.0, 0.0, 0.0, 0.0));
//    registry.emplace<Model_ENTT>(entity);*/
//
//    /*auto view = registry.view<PositionVector>();
//    for (const  entt::entity e : view)
//    {
//        view.get<PositionVector>(e).position;
//    }*/
//
//    /*Model_ENTT& model = registry.get<Model_ENTT>(entity);
//    model.model.Initialize();*/
//
//    /*PositionVector& pos_vector = registry.get<PositionVector>(entity);
//    registry.view<PositionVector>(entity)();
//    pos_vector.position = DirectX::XMVectorSet(1.0, 1.0, 1.0, 0.0);*/
//}

//void Scene::AddSimpleCube(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader, DirectX::XMFLOAT3 pos)
//{
//    entt::entity e = SimpleGameObject(m_registry);
//
//    SetPosition(m_registry.get<PositionRotation>(e), pos);
//
//    SetRotation(m_registry.get<PositionRotation>(e), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
//
//    SetModel(m_registry.get<ObjectModel>(e), filePath, device, deviceContext, cb_vs_vertexshader);
//
//    UpdateBoundingBox(m_registry.get<PositionRotation>(e), m_registry.get<ObjectModel>(e));
//}
//
//void Scene::DrawSceneEntt(const DirectX::XMMATRIX& viewProjectionMatrix, DirectX::BoundingFrustum& f_culling)
//{
//    auto view = m_registry.view<PositionRotation, ObjectModel>();
//
//    for (entt::entity entity : view)
//    {
//        if (f_culling.Contains(view.get<ObjectModel>(entity).model.bounding_box) != DirectX::DISJOINT)
//        {
//            UpdatedModel up_model = view.get<ObjectModel>(entity).model;
//            DirectX::XMMATRIX worldMatrix = view.get<PositionRotation>(entity).worldMatrix;
//
//            up_model.Draw(worldMatrix, viewProjectionMatrix);
//        }
//    }
//
//
//
//
//}

//void Scene::Initialize(ModelLoader& model_loader) {
//
//    _model_loader = model_loader;
//
//    // перенести потом в другое место
//    entt::entity second_entity = TestNewGameObject(m_registry);
//
//    ComponentSystems::SetPosition(m_registry.get<TransformComponent>(second_entity), DirectX::XMFLOAT3(0.0f, 6.0f, 0.0f));
//    ComponentSystems::SetRotation(m_registry.get<TransformComponent>(second_entity), DirectX::XMFLOAT3(0.5f, 0.0f, 0.0f));
//    ComponentSystems::SetModel(m_registry.get<MeshComponent>(second_entity), _model_loader.GetModelById(0));
//    ComponentSystems::UpdateBoundingBox(m_registry.get<MeshComponent>(second_entity), m_registry.get<TransformComponent>(second_entity), _model_loader.GetModelById(1));
//
//}

//bool RenderableGameObject::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader) {
//    if (!model.Initialize(filePath, device, deviceContext, cb_vs_vertexshader))
//        return false;
//
//    this->SetPosition(0.0f, 0.0f, 0.0f);
//    this->SetRotation(0.0f, 0.0f, 0.0f);
//
//    bounding_box = model.GetBoundingBox();
//
//    this->UpdateMatrix();
//    return true;
//}
