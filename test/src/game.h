#pragma once

#include "core.h"
#include <memory>
 
class Game :public engine::Application {
public:
	bool Init()override;
	void Update(float delta)override;
	void Destory()override;

private:
	engine::Material m_material;
	std::unique_ptr<engine::Mesh> m_mesh;
	float m_offsetX = 0;
	float m_offsetY = 0;
};