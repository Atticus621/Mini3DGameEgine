#pragma once

#include "core.h"
#include <memory>

class TestObject :public engine::GameObject {
public:
	TestObject();
	void Update(float delta)override;

};