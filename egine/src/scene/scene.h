#pragma once
#include <string>
#include <vector>
#include <memory>

#include "game_object.h"

namespace engine {

	class GameObject;

	class Scene {
	public:

		void Update(float delta);
		void Clear();

		GameObject* CreateGameObject(const std::string& name = "GameObject",GameObject* m_parent=nullptr);
		template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<GameObject, T>>>
		T* CreateGameObject(const std::string& name = "GameObject",GameObject* parent=nullptr)
		{
			auto gameObject = new T();
			gameObject->SetName(name);
			SetParent(gameObject, parent);
			return gameObject;
		}
		bool SetParent(GameObject* child, GameObject* parent);
	private:
		std::vector<std::unique_ptr<GameObject>> m_gameObjects;
	};
}