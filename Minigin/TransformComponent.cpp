#include "TransformComponent.h"

const glm::vec3& dae::TransformComponent::GetPosition() const
{
	return m_position;
}

void dae::TransformComponent::SetPosition(const glm::vec3& position)
{ 
	m_position = position; 
}

void dae::TransformComponent::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void dae::TransformComponent::SetPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
}
