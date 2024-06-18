#pragma once
#include "Common.h"
#include <iostream>
class Button : public sf::Drawable {
private:
	bool is_pressed = false;
	bool should_click = false;
	sf::RectangleShape rect;
	sf::Text text;
	HandlerCheckReoults HandlerCheck(int x, int y, sf::RenderTarget& target) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Button() = delete;
	Button(sf::Font& font);
	void HandleMouseRealease(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target);
	void HandleMousePress(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target);
	bool Click();
	void setPosition(sf::Vector2f pos);
	void setSize(sf::Vector2f size);
	void setString(const std::string& string);
};