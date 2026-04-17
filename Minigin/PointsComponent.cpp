#include "PointsComponent.h"
#include "GameObject.h"

dae::PointsComponent::PointsComponent(GameObject* owner, int points = 0)
	:BaseComponent(owner),
	m_Points{ points }
{}

void dae::PointsComponent::Update(float deltaTime)
{
	deltaTime = 10; //remove once used (hopefully never)
}

void dae::PointsComponent::Render() const
{

}

int dae::PointsComponent::GetPoints()
{
	return m_Points;
}

void dae::PointsComponent::SetPoints(int newPoints)
{
	m_Points = newPoints;
	GetOwner()->PushEvent(GetOwner(), std::string{ "UpdatePoints" });
}