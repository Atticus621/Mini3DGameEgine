#include "component.h"

size_t engine::Component::nextTypeId=0;

engine::GameObject* engine::Component::GetOwner() const
{
	return m_owner;
}
