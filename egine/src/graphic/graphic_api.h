#pragma once

#include <memory>
#include <string>
#include <GLES3/gl3.h>
#include <vector>
namespace engine{
	class ShaderProgram;
	class Material;
	class Mesh;
	class GraphicAPI {
	public:
		std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
		std::shared_ptr<ShaderProgram>& GetDefaultShaderProgram();
		bool Init();
		void BindShaderProgram(ShaderProgram* shaderProgram);
		void BindMaterial(Material* material);

		GLuint CreateVBO(const std::vector<float>& vertices);
		GLuint CreateEBO(const std::vector<uint32_t>& indices);

		void SetClearCorlor(float r, float g, float b, float a);
		void ClearBuffer();

		void BindMesh(Mesh* mesh);
		void DrawMesh(Mesh* mesh);

	private:
		std::shared_ptr<ShaderProgram> m_defaultShaderProgram;
	};
}
