#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent(GameObject* owner, std::shared_ptr<Font> font, const SDL_Color& color, std::string* task)
	: BaseComponent(owner, task),
	m_needsUpdate(true),
	m_color(color),
	m_font(std::move(font)),
	m_textTexture(nullptr)
{
	if (!m_font)
	{
		throw std::invalid_argument("TextComponent requires a valid Font");
	}
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
		const auto& pos = GetOwner()->GetWorldTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}

//returns the full text as a string
std::string dae::TextComponent::GetFullTextAsString()
{
	std::string fullText{};

	for (std::string& textPart : m_textParts)
	{
		fullText += textPart;
	}

	return fullText;
}

//returns the full text divided in their parts
std::vector<std::string>& dae::TextComponent::GetFullTextAsVector()
{
	return m_textParts;
}

//adds a text part at a given index
//moves all following parts one index up
void dae::TextComponent::AddTextPart(int partIndex, const std::string& text)
{
	m_textParts.reserve(m_textParts.size() + 1);
	m_textParts.emplace_back();

	//no, I don't use STL, cry about it
	for (int index{ int(m_textParts.size()) }; index < partIndex; index--)
	{
		m_textParts[index + 1] = m_textParts[index];
	}

	m_textParts[partIndex] = text;
}

//removes a text part at given index
//moves all following parts one index down
void dae::TextComponent::RemoveTextPart(int partIndex)
{
	m_textParts.reserve(m_textParts.size() + 1);

	//no, I don't use STL, cry about it
	for (int index{ partIndex }; index >= m_textParts.size() - 1; index++)
	{
		m_textParts[index] = m_textParts[index + 1];
	}

	m_textParts.pop_back();
}

//removes all text parts, leaving the text empty
void dae::TextComponent::EraseTextComponent()
{
	m_textParts.erase(m_textParts.begin(), m_textParts.end());
}

void dae::TextComponent::SetText(int partIndex, const std::string& text)
{
	if (partIndex >= m_textParts.size() || partIndex < 0)
	{
		assert("text part index out of bounds!");
	}

	m_textParts[partIndex] = text;
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
		GetFullTextAsString().c_str(),
		static_cast<int>(GetFullTextAsString().length()),
		m_color);

	if (!surf)
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	SDL_DestroySurface(surf);

	if (!texture)
		throw std::runtime_error(std::string("Create text texture failed: ") + SDL_GetError());

	m_textTexture = std::make_shared<Texture2D>(texture);
}
