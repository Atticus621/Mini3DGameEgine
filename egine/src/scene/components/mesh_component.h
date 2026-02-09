#pragma once


#include "scene/component.h"
#include <memory>

namespace engine
{
	class Material;
	class Mesh;

	class MeshComponent :public Component
	{
	public:
		MeshComponent(const std::shared_ptr<Material>& material, const std::shared_ptr<Mesh>& mesh);
		void Update(float delta)override;
	private:
		std::shared_ptr<Material> m_material;
		std::shared_ptr<Mesh> m_mesh;

	};
}