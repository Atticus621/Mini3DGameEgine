#pragma once

#include "core.h"

class Game :public engine::Application {
public:
	bool Init()override;
	void Update(float delta)override;
	void Destory()override;

private:
	engine::Material m_material;
};