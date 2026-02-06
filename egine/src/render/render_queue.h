#pragma once
#include <vector>


namespace engine {

	class Material;
	class Mesh;
	class GraphicAPI;

	struct RenderCommand
	{
		Mesh* mesh{ nullptr };
		Material* material{ nullptr };
	};

	class RenderQueue {
	public:
		void Submit(const RenderCommand& renderCommand);
		void Draw(GraphicAPI& graphicAPI);

	private:
		std::vector<RenderCommand> m_renderCommands;
	};
}