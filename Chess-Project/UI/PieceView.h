#pragma once
#include"../BoardView.h"
#include<SFML/Graphics.hpp>
class PieceView
{
private:
	const PieceData& piece;
	sf::RectangleShape rect;
public:
	PieceView(const PieceData& piece, const sf::Vector2f& pos, int squareSize);
	void setPosition(const sf::Vector2f& pos);
	const sf::RectangleShape& getDrawable();
};

