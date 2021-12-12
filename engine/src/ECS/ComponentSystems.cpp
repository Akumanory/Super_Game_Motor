#include <motor/ECS/ComponentSystems.h>

void ComponentSystems::SetPosition(Entity& entity, DirectX::XMFLOAT3 pos) 
{
    auto& transform_comp = entity.GetComponent<TransformComponent>();
    transform_comp.position = pos;

    if (entity.HasComponent<MeshComponent>()) 
    {
        UpdateBoundingBox(entity);
    }
}

void ComponentSystems::SetRotation(Entity& entity, DirectX::XMFLOAT3 rot) 
{
    auto& transform_comp = entity.GetComponent<TransformComponent>();
    transform_comp.rotation = rot;

    if (entity.HasComponent<MeshComponent>()) 
    {
        UpdateBoundingBox(entity);
    }
}

void ComponentSystems::AjustRotation(Entity& entity, DirectX::XMFLOAT3 rot, float delta) 
{
    auto& transform_comp = entity.GetComponent<TransformComponent>();
    transform_comp.rotation.x += rot.x * delta;
    transform_comp.rotation.y += rot.y * delta;
    transform_comp.rotation.z += rot.z * delta;

    if (entity.HasComponent<MeshComponent>()) 
    {
        UpdateBoundingBox(entity);
    }
}

void ComponentSystems::SetModel(Entity& entity, ModelStruct& model) {
    auto& mesh_comp = entity.GetComponent<MeshComponent>();
    mesh_comp.model.meshes = model.meshes;
    mesh_comp.model.bounding_box = model.bounding_box;

    if (entity.HasComponent<MeshComponent>()) 
    {
        UpdateBoundingBox(entity);
    }
}

void ComponentSystems::UpdateBoundingBox(Entity& entity) 
{
    auto& mesh_comp = entity.GetComponent<MeshComponent>();
    const auto& transform_comp = entity.GetComponent<TransformComponent>();

    mesh_comp.model.bounding_box.Transform(mesh_comp.transformed_bounding_box, transform_comp.GetTransformMatrix());
}