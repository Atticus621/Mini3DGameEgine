#include "game_object.h"

void engine::GameObject::Update(float delta)
{
    for(auto& child:m_children) {
		if (child->IsAlive()) {
           child->Update(delta);
       }
        else{
           m_children.erase(std::remove_if(m_children.begin(), m_children.end(),
                [](const std::unique_ptr<GameObject>& child) { return !child->IsAlive(); }),
				m_children.end());
        }
	}
}

const std::string& engine::GameObject::GetName() const
{
	return m_name;
}

void engine::GameObject::SetName(const std::string& name)
{
	m_name = name;
}

bool engine::GameObject::IsAlive() const
{
    return m_isAlive;
}

void engine::GameObject::MarkForDestory()
{
	m_isAlive = false;
}

engine::GameObject* engine::GameObject::GetParent()
{
    return m_parent;
}
