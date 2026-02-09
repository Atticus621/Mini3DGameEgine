#pragma once

#include "core.h"
#include <memory>

class TestObject :public engine::GameObject {
public:
	TestObject();
	void Update(float delta)override;

private:
	engine::Material m_material;
	std::shared_ptr<engine::Mesh> m_mesh;
	float m_offsetX = 0;
	float m_offsetY = 0;
};