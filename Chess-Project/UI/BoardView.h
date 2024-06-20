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
	sf::RectangleShape m_board, m_promotion_bg;
	std::vector<MovesToDraw> m_legal_moves;
	std::unique_ptr<std::vector<sf::RectangleShape>> m_pieces = std::make_unique<std::vector<sf::RectangleShape>>();
	std::unique_ptr<std::vector<sf::CircleShape>> m_moves = std::make_unique<std::vector<sf::CircleShape>>();
	std::array<sf::RectangleShape,4> m_promotions;
	sf::Vector2f m_pos;
	bool m_move_selection_changed = true;
	bool m_size_or_position_changed = true;
	int m_selected = -1;
	float m_size;
	Move m_promotion_move = NullMove;
	static std::atomic_bool m_textures_loaded;
	HandlerCheckReoults HandlerCheck(int x, int y, sf::RenderTarget& target);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	static void LoadTextures();

public:
	static sf::Texture white_pawn;
	static sf::Texture black_pawn;
	static sf::Texture white_knight;
	static sf::Texture black_knight;
	static sf::Texture white_bishop;
	static sf::Texture black_bishop;
	static sf::Texture white_rook;
	static sf::Texture black_rook;
	static sf::Texture white_queen;
	static sf::Texture black_queen;
	static sf::Texture white_king;
	static sf::Texture black_king;
	static sf::Texture duck;
	static sf::Texture board_texture;
	static sf::Texture promotion_texture;
	std::unique_ptr<IGame> game;
	IBlockerPlayer* white;
	IBlockerPlayer* black;
	BoardView() = delete;
	BoardView(const BoardView&) = delete;
	BoardView(sf::Vector2f pos, float size, IBlockerPlayer* white, IBlockerPlayer* black, bool withBlockers = true);
	void Update(bool forceMovesUpdate = false);
	void SetPosition(sf::Vector2f m_pos);
	sf::Vector2f GetPosition();
	void SetSize(float m_size);
	float GetSize();
	void HandleMouseRealease(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target);
	void HandleMousePress(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target);
	void HandleMouseMove(sf::Event::MouseMoveEvent& e, sf::RenderTarget& target);
	~BoardView();
	BoardView& operator=(const BoardView& rhs);
};
