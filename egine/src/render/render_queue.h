#pragma once

#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/mat4x4.hpp>
#include "comman.h"

namespace engine {

	class Material;
	class Mesh;
	class GraphicAPI;

	struct RenderCommand
	{
		Mesh* mesh{ nullptr };
		Material* material{ nullptr };
		glm::mat4 modelMat{ 1.0f };
	};
	class RenderQueue {
	public:
		void Submit(const RenderCommand& renderCommand);
		void Draw(GraphicAPI& graphicAPI,const CameraData& cameraData,const std::vector<LightData>& lights);

	private:
		std::vector<RenderCommand> m_renderCommands;
	};
}