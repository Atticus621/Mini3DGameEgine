#pragma once


namespace engine
{
	class GameObject;
	class Component {
	public:
		virtual ~Component() = default;
		virtual void Update(float delta) = 0;

		GameObject* GetOwner() const;

	protected:
		GameObject* m_owner = nullptr;
		friend class GameObject;
	};
}