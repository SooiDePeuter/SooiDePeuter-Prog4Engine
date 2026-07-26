#pragma once
#include "BaseComponent.h"

namespace dae
{
	class HealthComponent final: public BaseComponent
	{
	public:
		
		HealthComponent(GameObject* owner, int health, std::string* task = nullptr);

		void Update(float deltaTime) override;
		void Render() const override;

		int GetHealth();
		void SetHealth(int newHealth);

	private:
		int m_Health;
	};
}