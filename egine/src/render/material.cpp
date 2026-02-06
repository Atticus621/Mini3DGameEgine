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
	if (m_params.find(name) == m_params.end()) {
		spdlog::info("{}  exists,will reset{}", name, val);
	}
	m_params[name] = val;
}

void engine::Material::Bind()
{
	if (!m_shaderProgram) {
		spdlog::error("shaderProgram is nullptr,can not bind material");
		return;
	}
	m_shaderProgram->Bind();

	for (auto& param : m_params) {
		m_shaderProgram->SetUniform(param.first, param.second);
	}
}

void engine::Material::Unbind()
{
}
