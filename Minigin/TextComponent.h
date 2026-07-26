#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "TransformComponent.h"
#include "BaseComponent.h"
#include <SDL3/SDL.h>

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final: public BaseComponent
	{
	public:
		void Update(float deltaTime) override;
		void Render() const override;

		std::string GetFullTextAsString();
		std::vector<std::string>& GetFullTextAsVector();
		void AddTextPart(int partIndex, const std::string& text);
		void RemoveTextPart(int partIndex);
		void EraseTextComponent();
		void SetText(int partIndex, const std::string& text);
		void SetColor(const SDL_Color& color);

		TextComponent(GameObject* owner,
			std::shared_ptr<Font> font,
			const SDL_Color& color = { 255, 255, 255, 255 },
			std::string* task = nullptr);
		~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		void RebuildTexture();

		bool m_needsUpdate{};
		std::vector<std::string> m_textParts{};
		SDL_Color m_color{ 255, 255, 255, 255 };

		std::shared_ptr<Font> m_font{};
		std::shared_ptr<Texture2D> m_textTexture{};
	};
}
