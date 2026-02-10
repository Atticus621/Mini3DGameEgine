#include "player_control_component.h"
#include <GLFW/glfw3.h>
#include "engine.h"
#include "input/input_manager.h"
#include "scene/game_object.h"
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

void engine::PlayerControlComponent::Update(float delta)
{
	auto& input = engine::Engine::GetInstance().GetInputManager();


	auto position = m_owner->GetPosition();
	auto rotation = m_owner->GetRotation();

	if (input.GetMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {

		auto mouseOldPos = input.GetMouseOldPosition();
		auto mouseCurrentPos = input.GetMouseCurrentPosition();

		float deltaX = mouseCurrentPos.x - mouseOldPos.x;
		float deltaY = mouseCurrentPos.y - mouseOldPos.y;

		float angleX = deltaY * m_sensitivity*delta;
		float angleY = -deltaX * m_sensitivity * delta;

		glm::quat deltaRotationY = glm::angleAxis(angleY, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec3 right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);
		glm::quat deltaRotationX = glm::angleAxis(angleX, right);

		rotation = glm::normalize(deltaRotationY * rotation * deltaRotationX);


		spdlog::info(" rotationX: {}, rotationY: {}", rotation.x, rotation.y);
		m_owner->SetRotation(rotation);
	}


	glm::vec3 forward = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 up = rotation * glm::vec3(0.0f, 1.0f, 0.0f);

	if (input.GetKeyPressed(GLFW_KEY_W)) {
		position += forward* m_moveSpeed * delta;
	}
	if (input.GetKeyPressed(GLFW_KEY_S)) {
		position -= forward * m_moveSpeed * delta;
	}
	if (input.GetKeyPressed(GLFW_KEY_A)) {
		position -= right * m_moveSpeed * delta;
	}
	if (input.GetKeyPressed(GLFW_KEY_D)) {
		position += right * m_moveSpeed * delta;
	}
	if (input.GetKeyPressed(GLFW_KEY_UP)) {
		position += up * m_moveSpeed * delta;
	}
	if (input.GetKeyPressed(GLFW_KEY_DOWN)) {
		position -= up * m_moveSpeed * delta;
	}
	m_owner->SetPosition(position);
}
