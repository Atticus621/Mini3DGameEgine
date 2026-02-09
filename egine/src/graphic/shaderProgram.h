#pragma once

#include <GLES3/gl3.h>
#include <unordered_map>
#include <string>
#include <glm/mat4x4.hpp>

namespace engine {
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
		void SetUniform(const std::string& name, const glm::mat4& mat4);
		void UnBind();
	private:
		std::unordered_map<std::string, GLint> m_uniformLocationCache;
		GLuint m_shaderProgramId = 0;
	};
}