#include "light_component.h"

void engine::LightComponent::Update(float delta)
{

}

void engine::LightComponent::SetCorlor(const glm::vec3& corlor)
{
    m_corlor = corlor;
}

glm::vec3 engine::LightComponent::GetCorlor() const
{
    return m_corlor;
}
