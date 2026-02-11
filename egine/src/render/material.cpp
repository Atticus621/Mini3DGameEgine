#include "material.h"
#include "graphic/shaderProgram.h"
#include <spdlog/spdlog.h>
#include "engine.h"
#include <nlohmann/json.hpp>

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

void engine::Material::SetParam(const std::string& name, std::shared_ptr<Texture>& texture)
{

	m_textureParams[name] = texture;
}

std::shared_ptr<engine::Material> engine::Material::Load(const std::string& path)
{
	auto contents = engine::Engine::GetInstance().GetFileSystem().LoadAssetsFileText(path);

	if (contents.empty()) {
		spdlog::error("Load material file {} failed",path);
		return nullptr;
	}

	nlohmann::json json = nlohmann::json::parse(contents);

	auto material = std::make_shared<Material>();

	if (!json.contains("shaders")) { spdlog::error("Material file {} does not contain shader field", path); return nullptr; }

	//set shaders and program
	auto shader = json["shaders"];
	std::string vertexShaderPath = shader.value("vertex", "");       if (vertexShaderPath.empty()) { spdlog::error("Material file {} shader field does not contain vertex shader path", path); return nullptr; }
	
	std::string fragmentShaderPath = shader.value("fragment", "");    if (fragmentShaderPath.empty()) {spdlog::error("Material file {} shader field does not contain fragment shader path", path);return nullptr;}
	auto& fs = engine::Engine::GetInstance().GetFileSystem();
	std::string vertexShaderSource = fs.LoadAssetsFileText(vertexShaderPath);
	std::string fragmentShaderSource = fs.LoadAssetsFileText(fragmentShaderPath);
	auto& graphicAPI = engine::Engine::GetInstance().GetGraphicAPI();
	auto shaderProgram = graphicAPI.CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
	material->SetShaderProgram(shaderProgram);

	//set params
	if (json.contains("params")) {
		auto params = json["params"];
		//float1 params
		if (params.contains("float")) {
			for (auto& p : params["float"]) {
				std::string name = p.value("name", ""); if (name.empty())spdlog::warn("material params float have no name");
				float val = p.value("value", 0.0f); 
				material->SetParam(name, val);
			}
		}
		//float2 params
		if (params.contains("float2")) {
			for (auto& p : params["float2"]) {
				std::string name = p.value("name", ""); if (name.empty())spdlog::warn("material params float2 have no name");
				float val0 = p.value("value0", 0.0f);
				float val1 = p.value("value1", 0.0f);
				material->SetParam(name, val0,val1);
			}
		}
		if (!params.contains("textures")) spdlog::warn("Materail can not find textures");
		else {
			auto textures = params["textures"];
			for (auto& texture : textures)
			{
				std::string name = texture.value("name", "");
				if (name.empty())spdlog::warn("Texture can not find name");
				std::string path = texture.value("path", "");
				auto texture = Texture::load(path);
				material->SetParam(name, texture);
			}

		}
	}   
	else {spdlog::warn("Material has not Params");}

	return material;
}


engine::ShaderProgram* engine::Material::GetShaderProgram() const
{
	return m_shaderProgram.get();
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
	for (auto& param : m_textureParams) {
		m_shaderProgram->SetTexture(param.first, param.second.get());
	}
}

void engine::Material::Unbind()
{
}
