#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "graphic/texture.h"

namespace engine {
	class ShaderProgram;
	class Material {
	public:
		void SetShaderProgram(std::shared_ptr<ShaderProgram>& shaderProgram);
		void SetParam(const std::string& name, float val);
		void SetParam(const std::string& name, float v0, float v1);
		void SetParam(const std::string& name, std::shared_ptr<Texture>& texture);
		ShaderProgram* GetShaderProgram()const;
		void Bind();
		void Unbind();
	private:
		std::shared_ptr<ShaderProgram> m_shaderProgram;
		std::unordered_map<std::string, float> m_f1Params;
		std::unordered_map<std::string, std::pair<float, float>> m_f2Params;
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_textureParams;
	};
}