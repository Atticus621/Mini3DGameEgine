#include "render_queue.h"
#include "render/material.h"
#include "render/mesh.h"
#include "graphic/graphic_api.h"
#include "graphic/shaderProgram.h"

void engine::RenderQueue::Submit(const RenderCommand& renderCommand)
{
	m_renderCommands.push_back(renderCommand);
}

void engine::RenderQueue::Draw(engine::GraphicAPI& graphicAPI,const CameraData& cameraData)
{
	for (auto& command : m_renderCommands) {

		graphicAPI.BindMaterial(command.material);
		auto shaderProgram = command.material->GetShaderProgram();
		shaderProgram->SetUniform("uModel", command.modelMat);
		shaderProgram->SetUniform("uView", cameraData.viewMat);
		shaderProgram->SetUniform("uProjection", cameraData.projectionMat);
		graphicAPI.BindMesh(command.mesh);
		graphicAPI.DrawMesh(command.mesh);
	}
	m_renderCommands.clear();



}
