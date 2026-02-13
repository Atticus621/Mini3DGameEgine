#pragma once

#include <GLES3/gl3.h>
#include <graphic/vertex_layout.h>
#include <memory>
#include <vector>
#include <string>

namespace engine {

	class Mesh {
	public:
		Mesh() = default;
		Mesh(const VertexLayout& vertexLayout, const std::vector<float>& vertices, const std::vector<uint32_t>& indices);
		Mesh(const VertexLayout& vertexLayout, const std::vector<float>& vertices);
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

		void Bind();
		void Draw();

		//static std::shared_ptr<Mesh> Load(const std::string& filePath);
		static std::shared_ptr<Mesh> CreateCubeMesh();
		void logInfo();
	private:
		VertexLayout m_vertexLayout;

		GLuint m_VAO=0;
		GLuint m_VBO=0;
		GLuint m_EBO=0;

		uint32_t m_vertexCount = 0;
		uint32_t m_indexCount = 0;
	};
}