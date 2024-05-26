#pragma once
#include<SFML/Graphics.hpp>

struct Textures {
public:
	sf::Texture whitePawn;
	sf::Texture blackPawn;
	sf::Texture whiteKnight;
	sf::Texture blackKnight;
	sf::Texture whiteBishop;
	sf::Texture blackBishop;
	sf::Texture whiteRook;
	sf::Texture blackRook;
	sf::Texture whiteQueen;
	sf::Texture blackQueen;
	sf::Texture whiteKing;
	sf::Texture blackKing;
};

void loadTextures(Textures& textures);
