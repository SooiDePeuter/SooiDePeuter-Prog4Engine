#pragma once
#include <string>
#include <memory>
#include "TransformComponent.h"
#include "BaseComponent.h"
#include <vector>
#include <type_traits>


namespace dae
{
	class Texture2D;
	class GameObject final
	{
		bool AddChild(GameObject* child);
		bool RemoveChild(GameObject* child);

		TransformComponent m_localTransform{};
		TransformComponent m_worldTransform{};
		std::shared_ptr<Texture2D> m_texture{};
		float m_TextureWidth{ 100 };
		float m_TextureHeight{ 100 };
		std::vector<std::unique_ptr<BaseComponent>> m_components{};

		GameObject* m_parent{};
		std::vector<GameObject*> m_children{};

		//dirty flag
		bool m_isPositionOutOfSync{ false };

		std::vector<BaseComponent*> m_Observers;

		static float DeltaTime;

	public:
		static void SetDeltaTime(float deltaTime)
		{
			DeltaTime = deltaTime;
		}

		void Update(float deltaTime);
		void Render() const;

		const TransformComponent& GetLocalTransform() const { return m_localTransform; }
		const TransformComponent& GetWorldTransform() const { return m_worldTransform; }
		glm::vec3 GetPosition() const 
		{
			return m_worldTransform.GetPosition();
		}

		void SetTexture(const std::string& filename);
		void SetTextureDimensions(float width, float height);
		float GetTextureWidth() const;
		float GetTextureHeight() const;
		void SetLocalPosition(float x, float y);


		template<typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<BaseComponent, T>, "T must derive from Component");

			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			m_components.emplace_back(std::move(component));
		}

		template<typename T>
		void RemoveComponent(T* component)
		{
			static_assert(std::is_base_of_v<BaseComponent, T>, "T must derive from Component");
			if (!component) return;
			if (component->GetOwner() != this) return; //I don't want to be able to delete component from other GO
			component->m_markedForRemoval = true;
		}

		template<typename T>
		T* GetComponentByTask(const std::string* task = nullptr)
		{
			static_assert(std::is_base_of_v<BaseComponent, T>, "T must derive from Component");
			for (std::unique_ptr<BaseComponent>& component : m_components)
			{
				//for every component I try to cast it to the type T, if cast success I return it
				T* casted = dynamic_cast<T*>(component.get());
				if (casted != nullptr && component.get()->GetTask() == task)
				{
					return casted;
				}
			}
			return nullptr;
		}

		template<typename T>
		T* GetComponent()
		{
			static_assert(std::is_base_of_v<BaseComponent, T>, "T must derive from Component");
			for (auto& component : m_components)
			{
				//for every component I try to cast it to the type T, if cast success I return it
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
			// GetComponent is not const, but I want to be able to call it from a const function
			GameObject* ConstThis = const_cast<GameObject*>(this);
			T* component = ConstThis->GetComponent<T>();

			if (component != nullptr)
			{
				return true;
			}

			return false;
		}

		//void CleanupRemovedComponents();

		bool SetParent(GameObject* parent);
		int GetChildCount() const;
		GameObject* GetChildAt(int index) const;
		GameObject* GetParent() const;

		void UpdatePosition();
		void SetDirtyFlag(bool flag);
		bool GetDirtyFlag() const;

		float movementSpeed{1.f};
		void MoveLeft()
		{
			SetLocalPosition(GetLocalTransform().GetPosition().x - movementSpeed * DeltaTime, GetLocalTransform().GetPosition().y);
		}
		void MoveRight()
		{
			SetLocalPosition(GetLocalTransform().GetPosition().x + movementSpeed * DeltaTime, GetLocalTransform().GetPosition().y);
		}
		void MoveUp()
		{
			SetLocalPosition(GetLocalTransform().GetPosition().x, GetLocalTransform().GetPosition().y - movementSpeed * DeltaTime);
		}
		void MoveDown()
		{
			SetLocalPosition(GetLocalTransform().GetPosition().x, GetLocalTransform().GetPosition().y + movementSpeed * DeltaTime);
		}

		void AddObserver(BaseComponent* observer);
		void RemoveObserver(BaseComponent* observer);
		void PushEvent(GameObject* sender, const std::string& type);
		void HandleEvent(GameObject* sender, const std::string* eventType);

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	};
}
