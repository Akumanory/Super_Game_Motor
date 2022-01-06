#include <motor/ECS/ComponentSystems.h>

void ComponentSystems::SetPosition(Entity& entity, DirectX::XMFLOAT3 pos) 
{
    auto& transform_comp = entity.GetComponent<TransformComponent>();
    transform_comp.world_position = pos;
}

void ComponentSystems::SetRotation(Entity& entity, DirectX::XMFLOAT3 rot) 
{
    auto& transform_comp = entity.GetComponent<TransformComponent>();
    transform_comp.world_rotation = rot;
}

void ComponentSystems::AjustRotation(Entity& entity, DirectX::XMFLOAT3 rot, float delta) 
{
    auto& transform_comp = entity.GetComponent<TransformComponent>();
    transform_comp.world_rotation.x += rot.x * delta;
    transform_comp.world_rotation.y += rot.y * delta;
    transform_comp.world_rotation.z += rot.z * delta;
}


void ComponentSystems::AjustPosition(Entity& entity, DirectX::XMFLOAT3 pos, float delta) 
{
    auto& transform_comp = entity.GetComponent<TransformComponent>();
    transform_comp.world_position.x += pos.x * delta;
    transform_comp.world_position.y += pos.y * delta;
    transform_comp.world_position.z += pos.z * delta;
}

void ComponentSystems::AjustPosition(Entity& entity, DirectX::XMVECTOR pos, float delta) {
    auto& transform_comp = entity.GetComponent<TransformComponent>();

    XMFLOAT3 temp_float = XMFLOAT3();
    XMStoreFloat3(&temp_float, pos);

    transform_comp.world_position.x += temp_float.x * delta;
    transform_comp.world_position.y += temp_float.y * delta;
    transform_comp.world_position.z += temp_float.z * delta;
}

void ComponentSystems::SetModel(Entity& entity, ModelStruct& model) {
    auto& mesh_comp = entity.GetComponent<MeshComponent>();
    mesh_comp.model.meshes = model.meshes;
    mesh_comp.model.bounding_box = model.bounding_box;
    mesh_comp.model.model_name = model.model_name;
    mesh_comp.model.file_path = model.file_path;
}

void ComponentSystems::UpdateBoundingBox(Entity& entity) 
{
    if (entity.HasComponent<TransformComponent>() && entity.HasComponent<MeshComponent>()) 
    {
        auto& mesh_comp = entity.GetComponent<MeshComponent>();
        //const auto& transform_comp = entity.GetComponent<TransformComponent>();
        mesh_comp.model.bounding_box.Transform(mesh_comp.transformed_bounding_box, GetTransformMatrix(entity));
        if (entity.HasComponent<ChildsComponent>()) {
            auto childs = entity.GetComponent<ChildsComponent>().child_entities;
            for (auto&& i : childs) {
                UpdateBoundingBox(i);
            }
        }
    }
}

void ComponentSystems::UpdateBoundingFrustum(Entity& entity) 
{
    if (entity.HasComponent<TransformComponent>() && entity.HasComponent<CameraComponent>()) 
    {
        auto& camera_comp = entity.GetComponent<CameraComponent>();
        camera_comp.camera.UpdateProjection();
        auto& frustum = camera_comp.camera.GetFrustum();
        frustum.Transform(frustum, entity.GetComponent<TransformComponent>().GetLocalTransformMatrix());
    }
}

//void ComponentSystems::SetChildEntity(Entity parent, Entity& child) 
//{
//    if (parent.HasComponent<TransformComponent>() && child.HasComponent<TransformComponent>()) 
//    {
//        if (!child.HasComponent<ParentComponent>()) {
//            // Нельязя что бы реебнок был зависи от двух родителей, но родитель может являтся доччерним компонентом другоко компонента
//            child.AddComponent<ParentComponent>();
//            auto& parent_comp = child.GetComponent<ParentComponent>();
//            parent_comp.parent = parent;
//
//            if (!parent.HasComponent<ChildsComponent>()) {
//                parent.AddComponent<ChildsComponent>();
//            }
//            auto& childs_comp = parent.GetComponent<ChildsComponent>();
//            childs_comp.child_entities.emplace_back(child);
//        }
//    }
//}

DirectX::XMMATRIX ComponentSystems::GetTransformMatrix(Entity& entity) {
    if (entity.HasComponent<ParentComponent>()) 
    {
        auto child_transform = entity.GetComponent<TransformComponent>().GetLocalTransformMatrix();

        DirectX::XMMATRIX parent_transform = ParentTransformMatrix(entity.GetComponent<ParentComponent>().parent);

        return child_transform * parent_transform;
    }
    return entity.GetComponent<TransformComponent>().GetLocalTransformMatrix();
}

DirectX::XMMATRIX ComponentSystems::ParentTransformMatrix(Entity entity) 
{
    DirectX::XMMATRIX parent_transform = DirectX::XMMatrixIdentity();

    auto child_transform = entity.GetComponent<TransformComponent>().GetWorldTransformMatrix();
    if (entity.HasComponent<ParentComponent>()) {
        Entity parent = entity.GetComponent<ParentComponent>().parent;
        parent_transform = ParentTransformMatrix(parent);
    } 
    return child_transform * parent_transform;
}