#include "Button.h"

HandlerCheckReoults Button::HandlerCheck(int x, int y, sf::RenderTarget& target) const
{
	auto localPos = target.mapPixelToCoords(sf::Vector2i{ x,y });
	auto selfBounds = rect.getGlobalBounds();
	localPos -= sf::Vector2f{ selfBounds.left, selfBounds.top };
	return {
		(localPos.x >= 0 && localPos.y >= 0 && localPos.x <= selfBounds.width && localPos.y <= selfBounds.height),
		localPos
	};
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect);
	target.draw(text);
}

Button::Button(sf::Font& font)
{
	text.setFont(font);
	text.setCharacterSize(20);
	rect.setFillColor(sf::Color(166, 166, 166));
}

void Button::HandleMouseRealease(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target)
{
	auto check = HandlerCheck(e.x, e.y, target);
	is_pressed = false;
	if (check.isOk && e.button == sf::Mouse::Left) {
		should_click = true;
	}
	rect.setFillColor(sf::Color(166, 166, 166));
}

void Button::HandleMousePress(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target)
{
	auto check = HandlerCheck(e.x, e.y, target);
	if (!check.isOk || e.button != sf::Mouse::Left) return;
	is_pressed = true;
	rect.setFillColor(sf::Color(115, 115, 115));
}

bool Button::Click()
{
	if (should_click) {
		should_click = false;
		return true;
	}
	return false;
}

void Button::setPosition(sf::Vector2f pos)
{
	rect.setPosition(pos);
	auto size = rect.getSize();
	auto charSize = text.getCharacterSize();
	auto stringSize = text.getString().getSize() * 10;
	text.setPosition(sf::Vector2f{ pos.x + size.x / 2 - stringSize / 2 , pos.y + size.y / 2 - charSize / 2 });
}

void Button::setSize(sf::Vector2f size)
{
	rect.setSize(size);
	auto pos = rect.getPosition();
	auto charSize = text.getCharacterSize();
	auto stringSize = text.getString().getSize() * 10;
	text.setPosition(sf::Vector2f{ pos.x + size.x / 2 - stringSize / 2 , pos.y + size.y / 2 - charSize / 2 });
}

void Button::setString(const std::string& string)
{
	this->text.setString(string);
	auto rectBounds = rect.getLocalBounds();
	auto charSize = text.getCharacterSize();
	auto stringSize = string.size() * 10;
	text.setPosition(sf::Vector2f{ rectBounds.left + rectBounds.width / 2 - stringSize / 2 , rectBounds.top + rectBounds.height / 2 - charSize / 2 });

}
