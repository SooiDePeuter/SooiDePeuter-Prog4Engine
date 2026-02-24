#pragma once
#include <string>
#include <memory>
#include "TransformComponent.h"
#include <cassert>

namespace dae
{
	class Texture2D;
	class GameObject final
	{
	public:
		void FixedUpdate(float deltaTime);
		void Update(float deltaTime);
		void Render() const;

		glm::vec3 GetPosition() const;
		TransformComponent& GetTransform();
		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);


		template<typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<BaseComponent, T>, "T must derive from BaseComponent");
			T* existingComponent = GetComponent<T>();
			if (existingComponent != nullptr)
			{
				return;
			}

			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			m_components.emplace_back(std::move(component));
		}

		template<typename T>
		void RemoveComponent(T* component)
		{
			static_assert(std::is_base_of_v<BaseComponent, T>, "T must derive from BaseComponent");
			if (!component)
			{
				return;
			}
			if (component->GetOwner() != this)
			{
				return;
			}
			component->m_markedForRemoval = true;
		}

		template<typename T>
		T* GetComponent()
		{
			static_assert(std::is_base_of_v<BaseComponent, T>, "T must derive from BaseComponent");
			for (auto& component : m_components)
			{
				T* casted = dynamic_cast<T*>(component.get());
				if (casted != nullptr)
				{
					return casted;
				}
			}
			return nullptr;
		}

		template<typename T>
		bool HasComponent() const
		{
			GameObject* ConstThis = const_cast<GameObject*>(this);
			T* component = ConstThis->GetComponent<T>();

			if (component != nullptr)
			{
				return true;
			}

			return false;
		}

		void CleanupRemovedComponents();

		GameObject() = default;
		virtual ~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private: 
		TransformComponent m_transform{};
		std::shared_ptr<Texture2D> m_texture{};
		std::vector<std::unique_ptr<BaseComponent>> m_components{};
	};
}
