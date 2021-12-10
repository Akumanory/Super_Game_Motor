#include <motor/ECS/ComponentSystems.h>

void ComponentSystems::SetPosition(TransformComponent& transform_comp, DirectX::XMFLOAT3 pos) {
    transform_comp.position = pos;
}

void ComponentSystems::SetRotation(TransformComponent& transform_comp, DirectX::XMFLOAT3 rot) {
    transform_comp.rotation = rot;
}

static void AjustRotation(TransformComponent& transform_comp, DirectX::XMFLOAT3 rot) {
    transform_comp.rotation.x += rot.x;
    transform_comp.rotation.y += rot.y;
    transform_comp.rotation.y += rot.y;
}

void ComponentSystems::SetModel(MeshComponent& mesh_comp, ModelStruct& model) {
    mesh_comp.meshes = model.meshes;
}

void ComponentSystems::UpdateBoundingBox(MeshComponent& mesh_comp, TransformComponent& transform_comp, ModelStruct& model) 
{
    model.bounding_box.Transform(mesh_comp.bounding_box, transform_comp.GetTransformMatrix());
}