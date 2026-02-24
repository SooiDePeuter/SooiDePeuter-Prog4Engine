#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color)
	: BaseComponent(owner),
	m_needsUpdate(true),
	m_text(text),
	m_color(color),
	m_font(std::move(font)),
	m_textTexture(nullptr)
{
	

	if (!m_font)
		throw std::invalid_argument("TextComponent requires a valid Font");
}

void dae::TextComponent::Update(float deltaTime)
{
	//this is to prevent a warning,	delete if deltaTime is being used
	deltaTime = NULL;

	if (!m_IsActive)
	{
		return;
	}

	if (m_needsUpdate)
	{
		RebuildTexture();
		m_needsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	if (m_textTexture != nullptr)
	{
		const auto& pos = GetOwner()->GetTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextComponent::SetColor(const SDL_Color& color)
{ 
	m_color = color; 
	m_needsUpdate = true; 
}

void dae::TextComponent::RebuildTexture()
{
	const auto surf = TTF_RenderText_Blended(m_font->GetFont(),
		m_text.c_str(),
		static_cast<int>(m_text.length()),
		m_color);

	if (!surf)
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	SDL_DestroySurface(surf);

	if (!texture)
		throw std::runtime_error(std::string("Create text texture failed: ") + SDL_GetError());

	m_textTexture = std::make_shared<Texture2D>(texture);
}
