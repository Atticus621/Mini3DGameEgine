#pragma once
#include <string>
#include <vector>
#include <memory>

namespace engine {

	class GameObject {
	public:
		virtual ~GameObject();

		virtual void Update(float delta);
		const std::string& GetName()const;
		void SetName(const std::string& name);
		bool IsAlive()const;
		void MarkForDestory();
		GameObject* GetParent();

	protected:
		GameObject() = default;

	private:
		std::string m_name;
		GameObject* m_parent = nullptr;
		std::vector<std::unique_ptr<GameObject>> m_children;
		bool m_isAlive = true;
		friend class Scene;
	};
}