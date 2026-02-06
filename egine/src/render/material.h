#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace engine {
	class ShaderProgram;
	class Material {
	public:
		void SetShaderProgram(std::shared_ptr<ShaderProgram>& shaderProgram);
		void SetParam(const std::string& name, float val);
		void Bind();
		void Unbind();
	private:
		std::shared_ptr<ShaderProgram> m_shaderProgram;
		std::unordered_map<std::string, float> m_params;
	};
}