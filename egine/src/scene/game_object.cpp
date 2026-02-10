#include "game_object.h"

void engine::GameObject::Update(float delta)
{
    for (auto& component : m_components) {
		component->Update(delta);
    }
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

void engine::GameObject::SetPosition(const glm::vec3& position)
{
    m_position = position;
}

void engine::GameObject::SetRotation(const glm::vec3& rotation)
{
	m_rotation = rotation;
}

void engine::GameObject::SetScale(const glm::vec3& scale)
{
	m_scale = scale;
}

void engine::GameObject::AddComponent(engine::Component* component)
{
	m_components.emplace_back(component);
	component->m_owner = this;
}

bool engine::GameObject::IsAlive() const
{
    return m_isAlive;
}

glm::mat4 engine::GameObject::GetLocalTransform() const
{
    glm::mat4 mat = glm::translate(glm::mat4(1.0f), m_position);

    mat = glm::rotate(mat, m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    mat = glm::rotate(mat, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	mat = glm::rotate(mat, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	mat = glm::scale(mat, m_scale);

	return mat;
}

glm::mat4 engine::GameObject::GetWorldTransform() const
{
    if (m_parent) {
		return m_parent->GetWorldTransform() * GetLocalTransform();
    }
    else {
		return GetLocalTransform();
    }
}

void engine::GameObject::MarkForDestory()
{
	m_isAlive = false;
}

engine::GameObject* engine::GameObject::GetParent()
{
    return m_parent;
}

glm::vec3 engine::GameObject::GetPosition() const
{
    return m_position;
}

glm::vec3 engine::GameObject::GetRotation() const
{
    return m_rotation;
}

glm::vec3 engine::GameObject::GetScale() const
{
    return m_scale;
}
