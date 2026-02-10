#pragma once


#include "scene/component.h"
#include "glm/mat4x4.hpp"

namespace engine
{
	class CameraComponent : public Component
	{
	public:
		void Update(float delta) override;

		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix() const;

	};

}