#pragma once

#include <GLES3/gl3.h>
#include <unordered_map>
#include <string>
#include <glm/mat4x4.hpp>

namespace engine {
	class Texture;

	class ShaderProgram {
	public:
		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		explicit ShaderProgram(GLuint shaderProgramId);
		~ShaderProgram();

		void Bind();
		GLint GetUniforLocation(const std::string& name);
		GLuint GetShaderProgramID();
		void SetUniform(const std::string& name, float val);
		void SetUniform(const std::string& name, float v0, float v1);
		void SetUniform(const std::string& name, const glm::vec3& v3);
		void SetUniform(const std::string& name, const glm::mat4& mat4);
		void SetTexture(const std::string& name, const Texture* texture);
		void UnBind();
	private:
		std::unordered_map<std::string, GLint> m_uniformLocationCache;
		GLuint m_shaderProgramId = 0;
		int m_textureUnitCounter = 0; // 用于跟踪已绑定的纹理单元数量
	};
}