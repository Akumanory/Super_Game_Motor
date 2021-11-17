#include <motor/ECS/ComponentSystems.h>

void SetPosition(TransformComponent& transform_comp, DirectX::XMFLOAT3 pos) 
{
    transform_comp.position = pos;
}

void SetRotation(TransformComponent& transform_comp, DirectX::XMFLOAT3 rot) 
{
    transform_comp.rotation = rot;
}

void SetModel(MeshComponent& mesh_comp, ModelStruct& model)
{
    mesh_comp.meshes = model.meshes;
}

void ComponentSystems::UpdateBoundingBox(MeshComponent& mesh_comp, TransformComponent& transform_comp, ModelStruct& model) 
{
    model.bounding_box.Transform(mesh_comp.bounding_box, transform_comp.GetTransformMatrix());
}