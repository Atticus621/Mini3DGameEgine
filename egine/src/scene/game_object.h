#pragma once
#include <string>
#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include "scene/component.h"
#include <spdlog/spdlog.h>

namespace engine {

	class Scene;
	class GameObject {
	public:
		virtual ~GameObject() = default;

		virtual void Update(float delta);

		void MarkForDestory();
		void SetActive(bool flag);
		void SetName(const std::string& name);
		void SetPosition(const glm::vec3& position=glm::vec3(0.0f,0.0f,0.0f));
		void SetRotation(const glm::quat& rotation);
		void SetScale(const glm::vec3& scale);
		void AddComponent(Component* component);
		template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<Component, T>>>
		T* GetComponent()
		{
			size_t typeId = Component::StaticType<T>();
			for (auto& component : m_components) {
				if (component->GetType() == typeId) {
					return static_cast<T*>(component.get());
				}
			}
			return nullptr;
		}

		const std::string& GetName()const;
		GameObject* GetParent();
		glm::vec3 GetLocalPosition()const;
		glm::vec3 GetWorldPosition()const;
		glm::quat GetRotation()const;
		glm::vec3 GetScale()const;
		bool IsAlive()const;
		bool IsActive()const;
		glm::mat4 GetLocalTransform()const;
		glm::mat4 GetWorldTransform()const;
		static GameObject* LoadGLTF(const std::string& path);
		GameObject* GetChild(const std::string& name);
		Scene* GetScene();
		bool SetParent(GameObject* parent);
	protected:
		GameObject() = default;

	private:
		std::string m_name;
		GameObject* m_parent = nullptr;
		std::vector<std::unique_ptr<GameObject>> m_children;
		std::vector<std::unique_ptr<Component>> m_components;
		bool m_isAlive = true;
		bool m_isActive = true;
		Scene* m_scene = nullptr;
		friend class Scene;
	private:
		glm::vec3 m_position{ 0.0f,0.0f,0.0f };
		glm::quat m_rotation{ 1.0f,0.0f,0.0f,0.0f };
		glm::vec3 m_scale{ 1.0f,1.0f,1.0f };
	};



};
