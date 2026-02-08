#include "scene.h"

void engine::Scene::Update(float delta)
{
    for (auto& child : m_gameObjects) {
        if (child->IsAlive()) {
            child->Update(delta);
        }
        else {
            m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),
                [](const std::unique_ptr<GameObject>& child) { return !child->IsAlive(); }),
                m_gameObjects.end());
        }
    }
}

void engine::Scene::Clear()
{
    m_gameObjects.clear();
}

engine::GameObject* engine::Scene::CreateGameObject(const std::string& name,engine::GameObject* parent)
{
	auto gameObject = new GameObject();
    gameObject->SetName(name);
	SetParent(gameObject, parent);
    return gameObject;
}

bool engine::Scene::SetParent(GameObject* object, GameObject* parent)
{
	bool result = false;
	auto currentParent = object->GetParent();

    if (parent == nullptr) {
      
        if (currentParent) {
            auto it = std::find_if(currentParent->m_children.begin(), currentParent->m_children.end(),
                [object](const std::unique_ptr<GameObject>& obj) { return obj.get() == object; });
			if (it != currentParent->m_children.end()) {
                currentParent->m_children.erase(it);
                m_gameObjects.push_back(std::move(*it));
                object->m_parent = nullptr;
				result = true;
            }
        }
        else {
            auto it = std::find_if(m_gameObjects.begin(), m_gameObjects.end(),
				[object](const std::unique_ptr<GameObject>& obj) { return obj.get() == object; });
            if (it == m_gameObjects.end()) {
				std::unique_ptr<GameObject> objHolder(object);
                m_gameObjects.push_back(std::move(objHolder));
                object->m_parent = nullptr;
                result = true;
			}
        }
    }
    else {
        if (currentParent) {
            auto it = std::find_if(currentParent->m_children.begin(), currentParent->m_children.end(),
                [object](const std::unique_ptr<GameObject>& obj) { return obj.get() == object; });
            if (it != currentParent->m_children.end()) {
                bool found = false;
                auto currentElement = parent;
                while (currentElement) {
                    if (currentElement == object) {
                        found = true;
                        break;
                    }
                    currentElement = currentElement->GetParent();
                }
                if (!found) {
                    currentParent->m_children.erase(it);
                    parent->m_children.push_back(std::move(*it));
                    object->m_parent = parent;
                    result = true;
                }
            }

        }
        else {
            auto it = std::find_if(m_gameObjects.begin(), m_gameObjects.end(),
                [object](const std::unique_ptr<GameObject>& obj) { return obj.get() == object; });
            if (it == m_gameObjects.end()) {
                std::unique_ptr<GameObject> ptr(object);
                parent->m_children.push_back(std::move(ptr));
                object->m_parent = parent;
                result = true;
            }
            else {
                bool found = false;
                auto currentElement = parent;
                while (currentElement) {
                    if (currentElement == object) {
                        found = true;
                        break;
                    }
                    currentElement = currentElement->GetParent();
                }
                if (!found) {
                    parent->m_children.push_back(std::move(*it));
                    object->m_parent = parent;
                    m_gameObjects.erase(it);
                    result = true;
                }
            }
        }

    }
	return result;

}
