#include "camera_component.h"
#include "scene/game_object.h"

void engine::CameraComponent::Update(float delta)
{
}

glm::mat4 engine::CameraComponent::GetViewMatrix() const
{
	return(glm::inverse(GetOwner()->GetWorldTransform()));
}

glm::mat4 engine::CameraComponent::GetProjectionMatrix() const
{
    return glm::mat4(1.0f);
}
