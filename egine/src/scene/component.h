#pragma once


namespace engine
{
	class GameObject;
	class Component {
	public:
		virtual ~Component() = default;
		virtual void Update(float delta) = 0;
		virtual size_t GetType() const = 0;

		GameObject* GetOwner() const;
		template<typename T>
		static size_t StaticType() {
			static size_t typeId = nextTypeId++;
			return typeId;
		};
	protected:
		GameObject* m_owner = nullptr;
		friend class GameObject;
	private:
		static size_t nextTypeId;
	protected:
#define COMPONENT(ComponetClass) \
	public:\
		static size_t TypeId()  { return Component::StaticType<ComponetClass>(); };\
		size_t GetType() const override { return TypeId(); };

	};
}