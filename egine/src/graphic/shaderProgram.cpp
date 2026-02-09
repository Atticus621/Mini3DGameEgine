#include "shaderProgram.h"
#include "spdlog/spdlog.h"
#include "glm/gtc/type_ptr.hpp"

engine::ShaderProgram::ShaderProgram(GLuint shaderProgramId):m_shaderProgramId(shaderProgramId)
{
}

engine::ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_shaderProgramId);
}

void engine::ShaderProgram::Bind()
{

	glUseProgram(m_shaderProgramId);

}

GLint engine::ShaderProgram::GetUniforLocation(const std::string& name)
{
	auto it = m_uniformLocationCache.find(name);
	if ( it!= m_uniformLocationCache.end()) {
		return it->second;
	}
	else
	{
		GLuint location = glGetUniformLocation(m_shaderProgramId, name.c_str());
		if (location == GL_INVALID_INDEX) {
			spdlog::error(" uniform {} can not find ", name);
		}
		else {
			m_uniformLocationCache[name] = location;
		}
		return location;

	}

}

GLuint engine::ShaderProgram::GetShaderProgramID()
{
	return m_shaderProgramId;
}

void engine::ShaderProgram::SetUniform(const std::string& name, float val)
{
	GLint location = GetUniforLocation(name);
	glUniform1f(location, val); 
}

void engine::ShaderProgram::SetUniform(const std::string& name, float v0, float v1)
{
	GLint location = GetUniforLocation(name);
	glUniform2f(location, v0,v1);
}

void engine::ShaderProgram::SetUniform(const std::string& name, const glm::mat4& mat4)
{
	GLint location = GetUniforLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
}

void engine::ShaderProgram::UnBind()
{
	glUseProgram(0);
}
