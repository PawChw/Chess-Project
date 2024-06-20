#pragma once
#include "Common.h"

class Checkmark : public sf::Drawable {
private:
	bool is_pressed = false;
	bool is_checked = false;
	sf::RectangleShape rect;
	sf::RectangleShape indicator;
	sf::Text text;
	HandlerCheckReoults HandlerCheck(int x, int y, sf::RenderTarget& target) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void UpdateValue();
public:
	Checkmark() = delete;
	Checkmark(sf::Font& font);
	void SetChecked(bool value);
	bool Checked();
	void HandleMouseRealease(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target);
	void HandleMousePress(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target);
	void SetPosition(sf::Vector2f m_pos);
	void SetSize(sf::Vector2f m_size);
	void SetString(const std::string& string);
};