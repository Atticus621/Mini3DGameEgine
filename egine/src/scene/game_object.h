#pragma once
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "scene/component.h"
#include <spdlog/spdlog.h>
namespace engine {


	class GameObject {
	public:
		virtual ~GameObject() = default;

		virtual void Update(float delta);

		void MarkForDestory();
		void SetName(const std::string& name);
		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);
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
			spdlog::warn("GameObject {} does not have component of type {}", m_name, typeid(T).name());
			return nullptr;
		}
		const std::string& GetName()const;
		GameObject* GetParent();
		glm::vec3 GetPosition()const;
		glm::vec3 GetRotation()const;
		glm::vec3 GetScale()const;
		bool IsAlive()const;

		glm::mat4 GetLocalTransform()const;
		glm::mat4 GetWorldTransform()const;

	protected:
		GameObject() = default;

	private:
		std::string m_name;
		GameObject* m_parent = nullptr;
		std::vector<std::unique_ptr<GameObject>> m_children;
		std::vector<std::unique_ptr<Component>> m_components;
		bool m_isAlive = true;
		friend class Scene;
	private:
		glm::vec3 m_position{ 0.0f,0.0f,0.0f };
		glm::vec3 m_rotation{ 0.0f,0.0f,0.0f };
		glm::vec3 m_scale{ 1.0f,1.0f,1.0f };
	};



};
