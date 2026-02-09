#pragma once
#include <vector>
#include <glm/mat4x4.hpp>

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
		void Draw(GraphicAPI& graphicAPI);

	private:
		std::vector<RenderCommand> m_renderCommands;
	};
}