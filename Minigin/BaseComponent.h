#pragma once
namespace dae
{
	class GameObject;

	class BaseComponent
	{
	public:
		
		virtual void Update(float deltaTime) = 0;
		virtual void Render() const = 0;

		GameObject* GetOwner() const 
		{ 
			return m_Owner;
		}

		bool GetIsActive() const 
		{
			return m_IsActive;
		}

		void SetIsActive(bool active) 
		{ 
			m_IsActive = active;
		}

		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		bool m_IsMarkedForRemoval{ false };

	protected:
		explicit BaseComponent(GameObject* owner) : m_Owner{ owner } {};

		GameObject* m_Owner{};
		bool m_IsActive{ true };
	};
}