#include "material.h"
#include "graphic/shaderProgram.h"
#include <spdlog/spdlog.h>

void engine::Material::SetShaderProgram(std::shared_ptr<engine::ShaderProgram>& shaderProgram)
{
	if (!shaderProgram)
	{
		spdlog::warn("sderProgram is nullptr,you have set materail's shaderProgram nullptr");
	}
	m_shaderProgram = shaderProgram;
}

void engine::Material::SetParam(const std::string& name, float val)
{
	if (m_f1Params.find(name) == m_f1Params.end()) {
		spdlog::info("{}  exists,will reset{}", name, val);
	}
	m_f1Params[name] = val;
}

void engine::Material::SetParam(const std::string& name, float v0, float v1)
{

	m_f2Params[name] = { v0,v1 };
}

void engine::Material::Bind()
{
	if (!m_shaderProgram) {
		spdlog::error("shaderProgram is nullptr,can not bind material");
		return;
	}
	m_shaderProgram->Bind();

	for (auto& param : m_f1Params) {
		m_shaderProgram->SetUniform(param.first, param.second);
	}
	for (auto& param : m_f2Params) {
		m_shaderProgram->SetUniform(param.first, param.second.first,param.second.second);
	}
}

void engine::Material::Unbind()
{
}
