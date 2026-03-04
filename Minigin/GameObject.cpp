#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <cassert>
#include <algorithm>


dae::GameObject::~GameObject() = default;

//for hierarchy changes, use SetParent
bool dae::GameObject::AddChild(GameObject* child)
{
	//no checks because it is a helper function
	m_children.push_back(child);
}

//for hierarchy changes, use SetParent(nullptr)
bool dae::GameObject::RemoveChild(GameObject* child)
{
	std::erase(m_children, child);
}


void dae::GameObject::Update(float deltaTime) {

	for (const auto& component : m_components)
	{
		component->Update(deltaTime);
	}

	if (m_isPositionOutOfSync)
	{
		UpdatePosition();
		SetDirtyFlag(false);
	}
}

void dae::GameObject::Render() const
{
	//old render
	if (m_texture)
	{
		const auto& pos = m_worldTransform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}

	for (const auto& component : m_components)
	{
		component->Render();
	}

}


void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetLocalPosition(float x, float y)
{
	m_localTransform.SetPosition(x, y, 0.0f);
}


dae::GameObject* dae::GameObject::GetParent(GameObject* parent) const
{
	//every gameobject should have a parent, so no check
	return m_parent;
}

bool dae::GameObject::SetParent(GameObject* parent)
{
	//validation
	if (parent == m_parent ||
		parent == this)
	{
		return false;
	}

	for (GameObject* child : m_children)
	{
		if (parent == child)
		{
			return false;
		}
	}

	//remove itself from previous parent
	if (m_parent)
	{
		for (int index{}; index < m_parent->GetChildCount(); index++)
		{
			m_parent->RemoveChild(this);
		}
	}

	//set parent
	m_parent = parent;

	//set itself as new parents child
	{
		m_parent->AddChild(this);
	}

	//update world transform
	SetDirtyFlag(true);
}

int dae::GameObject::GetChildCount() const
{
	return m_children.size();
}

//returns null if out of range
dae::GameObject* dae::GameObject::GetChildAt(int index) const
{
	if (index < m_children.size())
	{
		return m_children[index];
	}
	else
	{
		return NULL;
	}
}


void dae::GameObject::UpdatePosition()
{
	m_worldTransform.SetPosition(m_parent->GetPosition() + m_localTransform.GetPosition());

	for (GameObject* child : m_children)
	{
		child->UpdatePosition();
	}
}

void dae::GameObject::SetDirtyFlag(bool flag)
{
	m_isPositionOutOfSync = flag;

	for (GameObject* child : m_children)
	{
		child->SetDirtyFlag(flag);
	}
}

bool dae::GameObject::GetDirtyFlag() const
{
	return m_isPositionOutOfSync;
}
