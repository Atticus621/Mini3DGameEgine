#pragma once


#include "scene/component.h"
#include "glm/mat4x4.hpp"

namespace engine
{
	class CameraComponent : public Component
	{
	COMPONENT(CameraComponent);
	public:
		void Update(float delta) override;

		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix(float aspect) const;

	private:
		float m_fov = 45.0f;
		float nearPlane = 0.1f;
		float farPlane = 1000.0f;
	};

}