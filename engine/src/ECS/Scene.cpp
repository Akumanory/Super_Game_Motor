#include <motor/ECS/Scene.h>


void Scene::InitializeSceneEntt(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader) {

    first_entity = SimpleGameObject(m_registry);

    SetPosition(m_registry.get<PositionRotation>(first_entity), DirectX::XMFLOAT3(0.0f, 6.0f, 0.0f));

    SetRotation(m_registry.get<PositionRotation>(first_entity), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));

    SetModel(m_registry.get<ObjectModel>(first_entity), filePath, device, deviceContext, cb_vs_vertexshader);


    /*
    entt::entity entity = registry.create();
    registry.emplace<PositionVector>(entity, DirectX::XMVectorSet(0.0, 0.0, 0.0, 0.0));
    registry.emplace<Model_ENTT>(entity);*/

    /*auto view = registry.view<PositionVector>();
    for (const  entt::entity e : view) 
    {
        view.get<PositionVector>(e).position;
    }*/

    /*Model_ENTT& model = registry.get<Model_ENTT>(entity);
    model.model.Initialize();*/

    /*PositionVector& pos_vector = registry.get<PositionVector>(entity);
    registry.view<PositionVector>(entity)();
    pos_vector.position = DirectX::XMVectorSet(1.0, 1.0, 1.0, 0.0);*/
}

void Scene::DrawSceneEntt(const DirectX::XMMATRIX& viewProjectionMatrix) {
    auto view = m_registry.view<PositionRotation, ObjectModel>();

    for (entt::entity entity : view) {
        UpdatedModel up_model = view.get<ObjectModel>(entity).model;
        DirectX::XMMATRIX worldMatrix = view.get<PositionRotation>(entity).worldMatrix;

        up_model.Draw(worldMatrix, viewProjectionMatrix);
    }
}

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