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

		auto deltaX = mouseCurrentPos.x - mouseOldPos.x;
		auto deltaY = mouseCurrentPos.y - mouseOldPos.y;
		
		rotation.y += deltaX * m_sensitivity * delta;
		rotation.x += -deltaY * m_sensitivity * delta;

		spdlog::info(" rotationX: {}, rotationY: {}", rotation.x, rotation.y);
		m_owner->SetRotation(rotation);
	}

	glm::mat4 rotationMat = glm::mat4(1.0f);

	rotationMat = glm::rotate(rotationMat, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMat = glm::rotate(rotationMat, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMat = glm::rotate(rotationMat, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec3 forward = glm::normalize(glm::vec3(rotationMat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
	glm::vec3 right = glm::normalize(glm::vec3(rotationMat * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
	glm::vec3 up = glm::normalize(glm::vec3(rotationMat * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));

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
