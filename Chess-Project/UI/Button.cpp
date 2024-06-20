#include "Button.h"

HandlerCheckReoults Button::HandlerCheck(int x, int y, sf::RenderTarget& target) const
{
	auto localPos = target.mapPixelToCoords(sf::Vector2i{ x,y });
	auto selfBounds = m_rect.getGlobalBounds();
	localPos -= sf::Vector2f{ selfBounds.left, selfBounds.top };
	return {
		(localPos.x >= 0 && localPos.y >= 0 && localPos.x <= selfBounds.width && localPos.y <= selfBounds.height),
		localPos
	};
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_rect);
	target.draw(m_text);
}

Button::Button(sf::Font& font)
{
	m_text.setFont(font);
	m_text.setCharacterSize(20);
	m_rect.setFillColor(sf::Color(166, 166, 166));
}

void Button::HandleMouseRealease(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target)
{
	auto check = HandlerCheck(e.x, e.y, target);
	bool wasPressed = m_is_pressed;
	m_is_pressed = false;
	if (wasPressed && check.is_ok && e.button == sf::Mouse::Left) {
		m_is_checked = true;
	}
	m_rect.setFillColor(sf::Color(166, 166, 166));
}

void Button::HandleMousePress(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target)
{
	auto check = HandlerCheck(e.x, e.y, target);
	if (!check.is_ok || e.button != sf::Mouse::Left) return;
	m_is_pressed = true;
	m_rect.setFillColor(sf::Color(115, 115, 115));
}

bool Button::Click()
{
	if (m_is_checked) {
		m_is_checked = false;
		return true;
	}
	return false;
}

void Button::SetPosition(sf::Vector2f m_pos)
{
	m_rect.setPosition(m_pos);
	auto size = m_rect.getSize();
	auto charSize = m_text.getCharacterSize();
	auto stringSize = m_text.getString().getSize() * 10;
	m_text.setPosition(sf::Vector2f{ m_pos.x + size.x / 2 - stringSize / 2 , m_pos.y + size.y / 2 - charSize / 2 });
}

void Button::SetSize(sf::Vector2f m_size)
{
	m_rect.setSize(m_size);
	auto m_pos = m_rect.getPosition();
	auto charSize = m_text.getCharacterSize();
	auto stringSize = m_text.getString().getSize() * 10;
	m_text.setPosition(sf::Vector2f{ m_pos.x + m_size.x / 2 - stringSize / 2 , m_pos.y + m_size.y / 2 - charSize / 2 });
}

void Button::SetString(const std::string& string)
{
	this->m_text.setString(string);
	auto rectBounds = m_rect.getLocalBounds();
	auto charSize = m_text.getCharacterSize();
	auto stringSize = string.size() * 10;
	m_text.setPosition(sf::Vector2f{ rectBounds.left + rectBounds.width / 2 - stringSize / 2 , rectBounds.top + rectBounds.height / 2 - charSize / 2 });

}
