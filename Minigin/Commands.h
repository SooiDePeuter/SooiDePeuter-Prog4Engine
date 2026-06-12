#pragma once
#include "GameObject.h"
#include "PointsComponent.h"
#include "HealthComponent.h"
#include <memory>

namespace dae
{
	class BaseGameObjectCommand
	{
	public:
		BaseGameObjectCommand(std::unique_ptr<GameObject>& gameObject)
			:m_gameObject(gameObject.get())
		{
		}

		virtual void Execute() = 0;

	protected:
		std::unique_ptr<GameObject> m_gameObject;

	};

	class MoveLeft final : public BaseGameObjectCommand
	{
	public:
		MoveLeft(std::unique_ptr<GameObject>& gameObject)
			:BaseGameObjectCommand(gameObject)
		{
		}

		void Execute() override
		{
			m_gameObject->MoveLeft();
		}
	};

	class MoveRight final : public BaseGameObjectCommand
	{
	public:
		MoveRight(std::unique_ptr<GameObject>& gameObject)
			:BaseGameObjectCommand(gameObject)
		{
		}

		void Execute() override
		{
			m_gameObject->MoveRight();
		}
	};

	class MoveUp final : public BaseGameObjectCommand
	{
	public:
		MoveUp(std::unique_ptr<GameObject>& gameObject)
			:BaseGameObjectCommand(gameObject)
		{
		}

		void Execute() override
		{
			m_gameObject->MoveUp();
		}
	};

	class MoveDown final : public BaseGameObjectCommand
	{
	public:
		MoveDown(std::unique_ptr<GameObject>& gameObject)
			:BaseGameObjectCommand(gameObject)
		{
		}

		void Execute() override
		{
			m_gameObject->MoveDown();
		}
	};

	class TakeDamage final : public BaseGameObjectCommand
	{
	public:
		TakeDamage(std::unique_ptr<GameObject>& gameObject)
			:BaseGameObjectCommand(gameObject)
		{
		}

		void Execute() override
		{
			if (m_gameObject.get()->GetComponent<HealthComponent>() != NULL)
			{
				m_gameObject.get()->GetComponent<HealthComponent>()->SetHealth(
					m_gameObject.get()->GetComponent<HealthComponent>()->GetHealth() - 1);
			}
		}
	};

	class GetPoints final : public BaseGameObjectCommand
	{
	public:
		GetPoints(std::unique_ptr<GameObject>& gameObject)
			:BaseGameObjectCommand(gameObject)
		{
		}

		void Execute() override
		{
			if (m_gameObject.get()->GetComponent<PointsComponent>() != NULL)
			{
				m_gameObject.get()->GetComponent<PointsComponent>()->SetPoints(
					m_gameObject.get()->GetComponent<PointsComponent>()->GetPoints() + 10);
			}
		}
	};
}