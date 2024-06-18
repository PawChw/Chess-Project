#pragma once
#include "../Player.h"
#include <stdexcept>
#include "../Game.h"
#include <vector>
#include <array>
#include <memory>
#include <unordered_set>
#include <atomic>
#include "Common.h"

struct MovesToDraw {
	uint8_t from;
	uint8_t to;
	bool isCapture;
	bool isPromotion;
};

class BoardView : public sf::Drawable {
private:
	sf::RectangleShape board, promotionbg;
	sf::Texture boardTexture;
	sf::Texture promotionTexture;
	std::vector<MovesToDraw> legalMoves;
	std::unique_ptr<std::vector<sf::RectangleShape>> pieces = std::make_unique<std::vector<sf::RectangleShape>>();
	std::unique_ptr<std::vector<sf::CircleShape>> moves = std::make_unique<std::vector<sf::CircleShape>>();
	std::array<sf::RectangleShape,4> promotions;
	sf::Vector2f pos;
	bool selectedChanged = true;
	bool sizeOrPosChanged = true;
	int selected = -1;
	float size;
	Move promotionMove = NullMove;
	HandlerCheckReoults HandlerCheck(int x, int y, sf::RenderTarget& target);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	static std::atomic_bool texturesLoaded;
	static void loadTextures();

public:
	static sf::Texture whitePawn;
	static sf::Texture blackPawn;
	static sf::Texture whiteKnight;
	static sf::Texture blackKnight;
	static sf::Texture whiteBishop;
	static sf::Texture blackBishop;
	static sf::Texture whiteRook;
	static sf::Texture blackRook;
	static sf::Texture whiteQueen;
	static sf::Texture blackQueen;
	static sf::Texture whiteKing;
	static sf::Texture blackKing;
	static sf::Texture duck;
	std::unique_ptr<IGame> game;
	std::shared_ptr<IBlockerPlayer> white;
	std::shared_ptr<IBlockerPlayer> black;
	BoardView() = delete;
	BoardView(const BoardView&) = delete;
	BoardView(sf::Vector2f pos, float size, std::shared_ptr<IBlockerPlayer> white, std::shared_ptr<IBlockerPlayer> black, bool withBlockers = true);
	void Update();
	void setPosition(sf::Vector2f pos);
	sf::Vector2f getPosition();
	void setSize(float size);
	float getSize();
	void HandleMouseRealease(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target);
	void HandleMousePress(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target);
	void HandleMouseMove(sf::Event::MouseMoveEvent& e, sf::RenderTarget& target);
	~BoardView();
};
