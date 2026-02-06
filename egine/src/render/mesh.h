#pragma once

#include <GLES3/gl3.h>
#include <graphic/vertex_layout.h>

namespace engine {

	class Mesh {
	public:
		Mesh(const VertexLayout& vertexLayout, const std::vector<float>& vertices, const std::vector<uint32_t> indices);
		Mesh(const VertexLayout& vertexLayout, const std::vector<float>& vertices);
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

		void Bind();
		void Draw();

	private:
		VertexLayout m_vertexLayout;

		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_EBO;

		size_t m_vertexCount = 0;
		size_t m_indexCount = 0;
	};
}