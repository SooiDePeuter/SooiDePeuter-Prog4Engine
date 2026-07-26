#pragma once
#include <string>

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

		std::string* GetTask() const
		{
			return m_Task;
		}

		bool GetIsActive() const 
		{
			return m_IsActive;
		}

		void SetIsActive(bool active) 
		{ 
			m_IsActive = active;
		}

		virtual ~BaseComponent()
		{
			delete m_Task;
		};
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		bool m_IsMarkedForRemoval{ false };

	protected:
		explicit BaseComponent(GameObject* owner, std::string* task = nullptr) : m_Owner{owner}, m_Task{task} {};

		GameObject* m_Owner{};
		bool m_IsActive{ true };

		std::string* m_Task{};
	};
}