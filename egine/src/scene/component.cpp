#include "component.h"


engine::GameObject* engine::Component::GetOwner() const
{
	return m_owner;
}
