#pragma once
#include "GameObject.h"

namespace dae
{
	class BaseGameObjectCommand
	{
	public:
		BaseGameObjectCommand() = default;
		virtual void Execute() = 0;

	protected:
		GameObject* GetGameObject()
		{
			return m_gameObject;
		}
	private:
		GameObject* m_gameObject;

	};

	class MoveLeft final : public BaseGameObjectCommand
	{
	public:
		void Execute() override
		{
			GetGameObject()->MoveLeft();
		}
	};

	class MoveRight final : public BaseGameObjectCommand
	{
	public:
		void Execute() override
		{
			GetGameObject()->MoveRight();
		}
	};

	class MoveUp final : public BaseGameObjectCommand
	{
	public:
		void Execute() override
		{
			GetGameObject()->MoveUp();
		}
	};

	class MoveDown final : public BaseGameObjectCommand
	{
	public:
		void Execute() override
		{
			GetGameObject()->MoveDown();
		}
	};
}