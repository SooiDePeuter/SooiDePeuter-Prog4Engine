#include "HealthComponent.h"
#include "GameObject.h"

dae::HealthComponent::HealthComponent(GameObject* owner, int health, std::string* task)
	:BaseComponent(owner, task),
	m_Health{health}
{}

void dae::HealthComponent::Update(float deltaTime)
{
	deltaTime = 10; //remove once used (hopefully never)
}

void dae::HealthComponent::Render() const
{

}

int dae::HealthComponent::GetHealth()
{
	return m_Health;
}

void dae::HealthComponent::SetHealth(int newHealth)
{
	m_Health = newHealth;
	GetOwner()->PushEvent(GetOwner(), std::string{ "UpdateHealth" });
}