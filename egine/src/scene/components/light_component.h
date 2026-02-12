#pragma once
#include "scene/component.h"
#include "glm/vec3.hpp"

namespace engine {

	class LightComponent:public Component {
		COMPONENT(LightComponent);
	public:
		void Update(float delta)override;

		void SetCorlor(const glm::vec3& corlor);
		glm::vec3 GetCorlor()const;
	private:
		glm::vec3 m_corlor{1.0f,1.0f,1.0f};
	};
}