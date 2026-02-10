#pragma once

#include "scene/component.h"

namespace engine
{
	class PlayerControlComponent : public Component
	{
		COMPONENT(PlayerControlComponent);
	public:
		void Update(float delta) override;

	private:
		float m_moveSpeed = 2.0f;
		float m_sensitivity = 0.02f;
	};
}