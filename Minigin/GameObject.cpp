#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

void dae::GameObject::FixedUpdate(float deltaTime) 
{
	for (const auto& component : m_components)
	{
		if (component->GetIsActive())
			component->Update(deltaTime);
	}
}

void dae::GameObject::Update(float deltaTime) 
{
	for (const auto& component : m_components)
	{
		if (component->GetIsActive())
			component->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	if (m_texture)
	{
		const auto& pos = m_transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}

	for (const auto& c : m_components)
	{
		if (c->GetIsActive())
			c->Render();
	}
}

glm::vec3 dae::GameObject::GetPosition() const
{
	return m_transform.GetPosition();
}

dae::TransformComponent& dae::GameObject::GetTransform()
{
	return m_transform;
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}


void dae::GameObject::CleanupRemovedComponents()
{
	//if Lambda returns true, the element will be removed from the vector
	std::erase_if(m_components, [](const std::unique_ptr<BaseComponent>& c)
		{
			return c->m_IsMarkedForRemoval;
		});
}