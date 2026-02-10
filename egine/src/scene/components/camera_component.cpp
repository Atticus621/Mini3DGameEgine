#include "camera_component.h"
#include "scene/game_object.h"
#include "glm/gtc/matrix_transform.hpp"


void engine::CameraComponent::Update(float delta)
{
}

glm::mat4 engine::CameraComponent::GetViewMatrix() const
{
	return(glm::inverse(GetOwner()->GetWorldTransform()));
}

glm::mat4 engine::CameraComponent::GetProjectionMatrix(float aspect) const
{
	return (glm::perspective(glm::radians(m_fov), aspect, nearPlane, farPlane));
}
