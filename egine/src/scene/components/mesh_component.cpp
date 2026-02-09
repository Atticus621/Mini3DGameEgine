#include "mesh_component.h"
#include "render/mesh.h"
#include "render/material.h"
#include "render/render_queue.h"
#include "engine.h"
#include "scene/game_object.h"

engine::MeshComponent::MeshComponent(const std::shared_ptr<engine::Material>& material, const std::shared_ptr<engine::Mesh>& mesh)
	:m_material(material), m_mesh(mesh)
{

}

void engine::MeshComponent::Update(float delta)
{
	if (!m_material || !m_mesh) {
        return;
    }
    engine::RenderCommand rendercommand;
    rendercommand.material = m_material.get();
    rendercommand.mesh = m_mesh.get();
    rendercommand.modelMat = GetOwner()->GetWorldTransform();

    auto& renderQueue = engine::Engine::GetInstance().GetRenderQueue();
    renderQueue.Submit(rendercommand);
}
