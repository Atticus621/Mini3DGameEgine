#include "camera_component.h"
#include "scene/game_object.h"
#include "glm/gtc/matrix_transform.hpp"


void engine::CameraComponent::Update(float delta)
{
}

glm::mat4 engine::CameraComponent::GetViewMatrix() const
{
	glm::mat4 viewMat = glm::mat4(1.0f);
	viewMat = glm::mat4_cast(GetOwner()->GetRotation());
	viewMat = glm::translate(viewMat,GetOwner()->GetLocalPosition());

	if (m_owner->GetParent())
	{
		viewMat = GetOwner()->GetParent()->GetWorldTransform() * viewMat;
	}
	return(glm::inverse(viewMat));
}

glm::mat4 engine::CameraComponent::GetProjectionMatrix(float aspect) const
{
	return (glm::perspective(glm::radians(m_fov), aspect, nearPlane, farPlane));
}
