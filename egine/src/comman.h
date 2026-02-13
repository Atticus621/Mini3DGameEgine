#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace engine {
	struct CameraData
	{
		glm::mat4 viewMat{ 1.0f };
		glm::mat4 projectionMat{ 1.0f };
		glm::vec3 position{ 0.0f };
	};

	struct LightData {
		glm::vec3 position;
		glm::vec3 color;
	};
};