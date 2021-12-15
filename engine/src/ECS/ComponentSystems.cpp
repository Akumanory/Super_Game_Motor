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
    //const auto& transform_comp = entity.GetComponent<TransformComponent>();
    mesh_comp.model.bounding_box.Transform(mesh_comp.transformed_bounding_box, GetTransformMatrix(entity));
    if (entity.HasComponent<ChildsComponent>()) 
    {
        auto childs = entity.GetComponent<ChildsComponent>().child_entities;
        for (auto&& i : childs) 
        {
            UpdateBoundingBox(i);
        }
    }
}

void ComponentSystems::SetChildEntity(Entity* parent, Entity& child) 
{
    if (parent->HasComponent<TransformComponent>() && child.HasComponent<TransformComponent>()) 
    {
        if (!child.HasComponent<ParentComponent>()) {
            // Нельязя что бы реебнок был зависи от двух родителей, но родитель может являтся доччерним компонентом другоко компонента
            child.AddComponent<ParentComponent>();
            auto& parent_comp = child.GetComponent<ParentComponent>();
            parent_comp.parent = parent;

            if (!parent->HasComponent<ChildsComponent>()) {
                parent->AddComponent<ChildsComponent>();
            }
            auto& childs_comp = parent->GetComponent<ChildsComponent>();
            childs_comp.child_entities.emplace_back(child);
            UpdateBoundingBox(child);
        }
    }
}

DirectX::XMMATRIX ComponentSystems::GetTransformMatrix(Entity& entity) {
    if (entity.HasComponent<ParentComponent>()) 
    {
        auto child_transform = entity.GetComponent<TransformComponent>().GetTransformMatrix();
        DirectX::XMMATRIX parent_transform = DirectX::XMMatrixIdentity();
        if (entity.GetComponent<ParentComponent>().parent->HasComponent<ParentComponent>()) 
        {
            Entity* parent = entity.GetComponent<ParentComponent>().parent;
            parent_transform = GetTransformMatrix(*parent);
        }
        else 
        {
            parent_transform = entity.GetComponent<ParentComponent>().parent->GetComponent<TransformComponent>().GetTransformMatrix();
        }
        
        return child_transform * parent_transform;
    }

    return entity.GetComponent<TransformComponent>().GetTransformMatrix();
}