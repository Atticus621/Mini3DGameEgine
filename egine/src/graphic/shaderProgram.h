#pragma once

#include <GLES3/gl3.h>
#include <unordered_map>
#include <string>

namespace engine {
	class ShaderProgram {
	public:
		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		explicit ShaderProgram(GLuint shaderProgramId);
		~ShaderProgram();

		void Bind();
		GLuint GetUniforLocation(const std::string& name);
		GLuint GetShaderProgramID();
		void SetUniform(const std::string& name, float val);
		void UnBind();
	private:
		std::unordered_map<std::string, GLint> m_uniformLocationCache;
		GLuint m_shaderProgramId = 0;
	};
}