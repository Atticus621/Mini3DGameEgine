#include "shaderProgram.h"
#include "spdlog/spdlog.h"

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

GLuint engine::ShaderProgram::GetUniforLocation(const std::string& name)
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
			m_uniformLocationCache.at(name) = location;
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
	auto it = m_uniformLocationCache.find(name);
	GLint location;
	if (it != m_uniformLocationCache.end()) {
		location = it->second;
	}
	else {
		location = glGetUniformLocation(m_shaderProgramId, name.c_str());
		if (location == GL_INVALID_INDEX) {
			spdlog::error(" uniform {} can not find ", name);
			return; 
		}
		m_uniformLocationCache[name] = location; 
	}
	glUniform1f(location, val); 
}

void engine::ShaderProgram::UnBind()
{
	glUseProgram(0);
}
