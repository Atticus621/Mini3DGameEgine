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
		void SetParam(const std::string& name, float v0, float v1);

		ShaderProgram* GetShaderProgram()const;
		void Bind();
		void Unbind();
	private:
		std::shared_ptr<ShaderProgram> m_shaderProgram;
		std::unordered_map<std::string, float> m_f1Params;
		std::unordered_map<std::string, std::pair<float, float>> m_f2Params;
	};
}