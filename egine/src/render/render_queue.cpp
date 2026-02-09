#include "render_queue.h"
#include "render/material.h"
#include "render/mesh.h"
#include "graphic/graphic_api.h"
#include "graphic/shaderProgram.h"

void engine::RenderQueue::Submit(const RenderCommand& renderCommand)
{
	m_renderCommands.push_back(renderCommand);
}

void engine::RenderQueue::Draw(engine::GraphicAPI& graphicAPI)
{
	for (auto& command : m_renderCommands) {

		graphicAPI.BindMaterial(command.material);
		command.material->GetShaderProgram()->SetUniform("uModel", command.modelMat);
		graphicAPI.BindMesh(command.mesh);
		graphicAPI.DrawMesh(command.mesh);
	}
	m_renderCommands.clear();



}
