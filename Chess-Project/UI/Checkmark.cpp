#include "Checkmark.h"

HandlerCheckReoults Checkmark::HandlerCheck(int x, int y, sf::RenderTarget& target) const
{
	auto localPos = target.mapPixelToCoords(sf::Vector2i{ x,y });
	auto selfBounds = m_rect.getGlobalBounds();
	localPos -= sf::Vector2f{ selfBounds.left, selfBounds.top };
	return {
		(localPos.x >= 0 && localPos.y >= 0 && localPos.x <= selfBounds.width && localPos.y <= selfBounds.height),
		localPos
	};
}

void Checkmark::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_rect);
	target.draw(m_text);
	target.draw(m_indicator);
}

void Checkmark::UpdateValue()
{
	if (m_is_checked) {
		m_indicator.setFillColor(sf::Color::Blue);
	}
	else {
		m_indicator.setFillColor(sf::Color::White);
	}
}

Checkmark::Checkmark(sf::Font& font)
{
	m_text.setFont(font);
	m_text.setCharacterSize(20);
	m_rect.setFillColor(sf::Color::Black);
	m_indicator.setFillColor(sf::Color::White);
	m_indicator.setSize(sf::Vector2f{ 15, 15 });
}

void Checkmark::SetChecked(bool value)
{
	m_is_checked = value;
	UpdateValue();
}

bool Checkmark::Checked()
{
	return m_is_checked;
}

void Checkmark::HandleMouseRealease(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target)
{
	auto check = HandlerCheck(e.x, e.y, target);
	bool wasPressed = m_is_pressed;
	m_is_pressed = false;
	if (wasPressed && check.is_ok && e.button == sf::Mouse::Left) {
		m_is_checked = !m_is_checked;
	}
	UpdateValue();
	m_rect.setFillColor(sf::Color::Black);
}

void Checkmark::HandleMousePress(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target)
{
	auto check = HandlerCheck(e.x, e.y, target);
	if (!check.is_ok || e.button != sf::Mouse::Left) return;
	m_is_pressed = true;
	m_rect.setFillColor(sf::Color(26, 26, 26));
}

void Checkmark::SetPosition(sf::Vector2f m_pos)
{
	m_rect.setPosition(m_pos);
	auto size = m_rect.getSize();
	auto charSize = m_text.getCharacterSize();
	auto stringSize = m_text.getString().getSize() * 10;
	m_text.setPosition(sf::Vector2f{ m_pos.x + size.x / 2 - stringSize / 2 - 10, m_pos.y + size.y / 2 - charSize / 2 });
	m_indicator.setPosition(sf::Vector2f{ m_pos.x + size.x / 2 + stringSize / 2, m_pos.y + size.y / 2 - charSize / 3 });
}

void Checkmark::SetSize(sf::Vector2f size)
{
	m_rect.setSize(size);
	auto m_pos = m_rect.getPosition();
	auto charSize = m_text.getCharacterSize();
	auto stringSize = m_text.getString().getSize() * 10;
	m_text.setPosition(sf::Vector2f{ m_pos.x + size.x / 2 - stringSize / 2 - 10, m_pos.y + size.y / 2 - charSize / 2 });
	m_indicator.setPosition(sf::Vector2f{ m_pos.x + size.x / 2 + stringSize / 2, m_pos.y + size.y / 2 - charSize / 3 });
}

void Checkmark::SetString(const std::string& string)
{
	this->m_text.setString(string);
	auto rectBounds = m_rect.getLocalBounds();
	auto charSize = m_text.getCharacterSize();
	auto stringSize = string.size() * 10;
	m_text.setPosition(sf::Vector2f{ rectBounds.left + rectBounds.width / 2 - stringSize / 2 - 10, rectBounds.top + rectBounds.height / 2 - charSize / 2 });
	m_indicator.setPosition(sf::Vector2f{ rectBounds.left + rectBounds.width / 2 + stringSize / 2, rectBounds.top + rectBounds.height / 2 - charSize / 3 });

}
