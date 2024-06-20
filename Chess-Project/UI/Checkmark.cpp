#include "Checkmark.h"

HandlerCheckReoults Checkmark::HandlerCheck(int x, int y, sf::RenderTarget& target) const
{
	auto localPos = target.mapPixelToCoords(sf::Vector2i{ x,y });
	auto selfBounds = rect.getGlobalBounds();
	localPos -= sf::Vector2f{ selfBounds.left, selfBounds.top };
	return {
		(localPos.x >= 0 && localPos.y >= 0 && localPos.x <= selfBounds.width && localPos.y <= selfBounds.height),
		localPos
	};
}

void Checkmark::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect);
	target.draw(text);
	target.draw(indicator);
}

void Checkmark::UpdateValue()
{
	if (is_checked) {
		indicator.setFillColor(sf::Color::Blue);
	}
	else {
		indicator.setFillColor(sf::Color::White);
	}
}

Checkmark::Checkmark(sf::Font& font)
{
	text.setFont(font);
	text.setCharacterSize(20);
	rect.setFillColor(sf::Color::Black);
	indicator.setFillColor(sf::Color::White);
	indicator.setSize(sf::Vector2f{ 15, 15 });
}

void Checkmark::SetChecked(bool value)
{
	is_checked = value;
	UpdateValue();
}

bool Checkmark::Checked()
{
	return is_checked;
}

void Checkmark::HandleMouseRealease(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target)
{
	auto check = HandlerCheck(e.x, e.y, target);
	bool wasPressed = is_pressed;
	is_pressed = false;
	if (wasPressed && check.is_ok && e.button == sf::Mouse::Left) {
		is_checked = !is_checked;
	}
	UpdateValue();
	rect.setFillColor(sf::Color::Black);
}

void Checkmark::HandleMousePress(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target)
{
	auto check = HandlerCheck(e.x, e.y, target);
	if (!check.is_ok || e.button != sf::Mouse::Left) return;
	is_pressed = true;
	rect.setFillColor(sf::Color(26, 26, 26));
}

void Checkmark::SetPosition(sf::Vector2f m_pos)
{
	rect.setPosition(m_pos);
	auto size = rect.getSize();
	auto charSize = text.getCharacterSize();
	auto stringSize = text.getString().getSize() * 10;
	text.setPosition(sf::Vector2f{ m_pos.x + size.x / 2 - stringSize / 2 - 10, m_pos.y + size.y / 2 - charSize / 2 });
	indicator.setPosition(sf::Vector2f{ m_pos.x + size.x / 2 + stringSize / 2, m_pos.y + size.y / 2 - charSize / 3 });
}

void Checkmark::SetSize(sf::Vector2f size)
{
	rect.setSize(size);
	auto m_pos = rect.getPosition();
	auto charSize = text.getCharacterSize();
	auto stringSize = text.getString().getSize() * 10;
	text.setPosition(sf::Vector2f{ m_pos.x + size.x / 2 - stringSize / 2 - 10, m_pos.y + size.y / 2 - charSize / 2 });
	indicator.setPosition(sf::Vector2f{ m_pos.x + size.x / 2 + stringSize / 2, m_pos.y + size.y / 2 - charSize / 3 });
}

void Checkmark::SetString(const std::string& string)
{
	this->text.setString(string);
	auto rectBounds = rect.getLocalBounds();
	auto charSize = text.getCharacterSize();
	auto stringSize = string.size() * 10;
	text.setPosition(sf::Vector2f{ rectBounds.left + rectBounds.width / 2 - stringSize / 2 - 10, rectBounds.top + rectBounds.height / 2 - charSize / 2 });
	indicator.setPosition(sf::Vector2f{ rectBounds.left + rectBounds.width / 2 + stringSize / 2, rectBounds.top + rectBounds.height / 2 - charSize / 3 });

}
