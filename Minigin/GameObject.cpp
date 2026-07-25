#include "GameObject.h"
#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "PointsComponent.h"
#include "Texture2D.h"
#include <cassert>
#include <algorithm>


float dae::GameObject::DeltaTime{ 0.f };

void dae::GameObject::AddObserver(GameObject* observer, const std::string& task)
{
	//validation
	if (observer == nullptr || observer == this)
	{
		return;
	}
	for (Observer& listedObserver : m_Observers)
	{
		if (listedObserver.GameObject == observer)
		{
			listedObserver.Tasks->push_back(task);
			return;
		}
	}

	m_Observers.push_back({ observer , new std::vector<std::string>{task} });
}

void dae::GameObject::RemoveObserver(GameObject* observer)
{

	if (observer == nullptr || observer == this)
	{
		return;
	}
	for (int index{}; index < m_Observers.size(); index++)
	{
		if (m_Observers[index].GameObject == observer)
		{
			m_Observers.erase(m_Observers.begin() + index);
		}
	}
}

void dae::GameObject::PushEvent(GameObject* sender, const std::string& type)
{
	for (Observer& listedObserver : m_Observers)
	{
		listedObserver.GameObject->HandleEvent(sender, type, listedObserver.Tasks);
	}
}

void dae::GameObject::HandleEvent(GameObject* sender, const std::string& eventType, const std::vector<std::string>* observerTasks)
{
	//validation
	const int taskCount{ (int)observerTasks->size() };
	int invalidTaskCount{};

	for (const std::string& observerTask : *observerTasks)
	{
		if (eventType != observerTask)
		{
			invalidTaskCount++;
		}
	}

	if (taskCount == invalidTaskCount)
	{
		return;
	}

	if (eventType == "UpdateHealth" && HasComponent<TextComponent>())
	{
		TextComponent* component = GetComponent<TextComponent>();
		std::string text{ std::to_string(sender->GetComponent<HealthComponent>()->GetHealth()) };
		component->SetText(1, text);
	}
	else if (eventType == "UpdatePoints" && HasComponent<TextComponent>())
	{
		TextComponent* component = GetComponent<TextComponent>();
		std::string text{ std::to_string(sender->GetComponent<PointsComponent>()->GetPoints()) };
		component->SetText(1, text);
	}
}

dae::GameObject::~GameObject() = default;

//for hierarchy changes, use SetParent
bool dae::GameObject::AddChild(GameObject* child)
{
	//no checks because it is a helper function
	m_children.push_back(child);

	return true;
}

//for hierarchy changes, use SetParent(nullptr)
bool dae::GameObject::RemoveChild(GameObject* child)
{
	std::erase(m_children, child);

	return true;
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
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y, m_TextureWidth, m_TextureHeight);
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

void dae::GameObject::SetTextureDimensions(float width, float height)
{
	m_TextureWidth = width;
	m_TextureHeight = height;
}

float dae::GameObject::GetTextureWidth()
{
	return m_TextureWidth;
}

float dae::GameObject::GetTextureHeight()
{
	return m_TextureHeight;
}

void dae::GameObject::SetLocalPosition(float x, float y)
{
	m_localTransform.SetPosition(x, y, 0.0f);
	dae::GameObject::SetDirtyFlag(true);
}

//parent must be validated/defaulted after use of this function
dae::GameObject* dae::GameObject::GetParent() const
{
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
	
	m_parent->AddChild(this);
	

	//update world transform
	SetDirtyFlag(true);

	return true;
}

int dae::GameObject::GetChildCount() const
{
	return (int)m_children.size();
}

//returns nullptr if out of range
dae::GameObject* dae::GameObject::GetChildAt(int index) const
{
	if (index < m_children.size())
	{
		return m_children[index];
	}
	else
	{
		return nullptr;
	}
}


void dae::GameObject::UpdatePosition()
{
	if (m_parent)
	{
		m_worldTransform.SetPosition(m_parent->GetPosition() + m_localTransform.GetPosition());
	}
	else
	{
		m_worldTransform.SetPosition(m_localTransform.GetPosition());
	}

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
