#include "BoardView.h"

std::atomic_bool BoardView::m_textures_loaded = false;
sf::Texture BoardView::white_pawn;
sf::Texture BoardView::black_pawn;
sf::Texture BoardView::white_knight;
sf::Texture BoardView::black_knight;
sf::Texture BoardView::white_bishop;
sf::Texture BoardView::black_bishop;
sf::Texture BoardView::white_rook;
sf::Texture BoardView::black_rook;
sf::Texture BoardView::white_queen;
sf::Texture BoardView::black_queen;
sf::Texture BoardView::white_king;
sf::Texture BoardView::black_king;
sf::Texture BoardView::duck;

void BoardView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_board);
	for (auto& shape : *m_pieces) {
		target.draw(shape);
	}
	for (auto& shape : *m_moves) {
		target.draw(shape);
	}
	if (m_promotion_move != NullMove) {
		target.draw(m_promotion_bg);
		for (auto& shape : m_promotions) {
			target.draw(shape);
		}
	}
}

BoardView::BoardView(sf::Vector2f pos, float size, std::shared_ptr<IBlockerPlayer> white, std::shared_ptr<IBlockerPlayer> black, bool withBlockers) : m_size(size), m_pos(pos), white(white), black(black) {
	LoadTextures();
	m_board = sf::RectangleShape(sf::Vector2f{ size ,size });
	m_promotion_bg = sf::RectangleShape(sf::Vector2f{ size ,size/4 });
	m_board_texture.loadFromFile("Assets/board.png");
	m_promotion_texture.loadFromFile("Assets/PromotionBg.png");
	m_board.setPosition(pos);
	m_board.setTexture(&m_board_texture);
	m_promotion_bg.setPosition(pos);
	m_promotion_bg.setTexture(&m_promotion_texture);
	m_size_or_position_changed = true;
	m_promotions.fill(sf::RectangleShape(sf::Vector2f{ size / 4, size / 4 }));
	if (withBlockers)
		game = std::make_unique<BlockerGame>(this->white, this->black);
	else
		game = std::make_unique<Game>(this->white, this->black);
	Update();

}

void BoardView::Update()
{
	float squareSize = m_size / 8.f;
	sf::RectangleShape square(sf::Vector2f{ squareSize, squareSize });
	float circleRadius = squareSize / 6;
	float circleOffset = squareSize/2 - circleRadius;
	sf::CircleShape circle(circleRadius);
	if (game->state_changed || m_move_selection_changed || m_size_or_position_changed) {
		if(!m_moves->empty())
			m_moves->clear();
		std::unordered_set<uint16_t> drawnMoves;
		HumanPlayer* toMove = nullptr;
		HumanPlayer* to_move_blocker = nullptr;
		if (auto d = dynamic_cast<HumanPlayer*>(white.get()); d != nullptr)
		{
			if (d->move) toMove = d;
			if (d->blocker_move) to_move_blocker = d;
		}
		if (auto d = dynamic_cast<HumanPlayer*>(black.get()); d != nullptr)
		{
			if (d->move) toMove = d;
			if (d->blocker_move) to_move_blocker = d;
		}
		if (toMove && m_selected != -1) {
			auto& legalMoves = game->bd.GetLegalMoves();
			for (Move move : legalMoves) {
				if (move.from == m_selected) {
					if (!drawnMoves.insert((static_cast<uint16_t>(move.from) << 8) | move.to).second) continue;
					m_legal_moves.push_back({ move.from, move.to, static_cast<bool>(move.captured_piece), static_cast<bool>(move.promoted_to_piece_type) });
					if (move.captured_piece && move.promoted_to_piece_type)
						circle.setFillColor(sf::Color(150, 150, 100, 200));
					else if (move.captured_piece)
						circle.setFillColor(sf::Color(150, 100, 100, 200));
					else if (move.promoted_to_piece_type)
						circle.setFillColor(sf::Color(100, 150, 100, 200));
					else if (move.is_castle.toBool())
						circle.setFillColor(sf::Color(200, 200, 200, 200));
					else
						circle.setFillColor(sf::Color(100, 100, 100, 200));
					circle.setPosition(sf::Vector2f{ (m_pos.x + GetFile(move.to) * squareSize) + circleOffset, (m_pos.y + GetRank(move.to) * squareSize) + circleOffset });
					m_moves->push_back(circle);
				}
			}
			if (m_move_selection_changed && m_promotion_move != NullMove) {
				if (game->bd.is_white_to_move) {
					m_promotions[0].setTexture(&white_knight);
					m_promotions[1].setTexture(&white_bishop);
					m_promotions[2].setTexture(&white_rook);
					m_promotions[3].setTexture(&white_queen);
				}
				else {
					m_promotions[0].setTexture(&black_knight);
					m_promotions[1].setTexture(&black_bishop);
					m_promotions[2].setTexture(&black_rook);
					m_promotions[3].setTexture(&black_queen);
				}
			}
		}
		else if (to_move_blocker) {
			Bitboard available = ~(game->bd.GetAllPiecesBitboard());
			if(IsValidSquare(game->bd.blocker_square))
				BitboardHelpers::ClearBit(available, game->bd.blocker_square);
			Square sq;
			circle.setFillColor(sf::Color(100, 100, 150, 200));
			while (available) {
				sq = BitboardHelpers::GetAndClearIndexOfLSB(available);
				circle.setPosition(sf::Vector2f{ (m_pos.x + GetFile(sq) * squareSize) + circleOffset, (m_pos.y + GetRank(sq) * squareSize) + circleOffset });
				m_moves->push_back(circle);
			}
		}
		m_move_selection_changed = false;
	}
	if (game->state_changed || m_size_or_position_changed) {
		game->state_changed = true;
		if(!m_pieces->empty())
			m_pieces->clear();
		square.setFillColor(sf::Color::White);
		for (Square i = 0; i < 64; i++) {
			square.setPosition(sf::Vector2f{ m_pos.x + (GetFile(i) * squareSize), m_pos.y + (GetRank(i) * squareSize) });
			auto piece = game->bd.GetPieceOnSquare(i);
			switch (piece)
			{
			case PieceUtils::White | PieceUtils::Pawn: square.setTexture(&white_pawn); break;
			case PieceUtils::White | PieceUtils::Knight: square.setTexture(&white_knight); break;
			case PieceUtils::White | PieceUtils::Bishop: square.setTexture(&white_bishop); break;
			case PieceUtils::White | PieceUtils::Rook: square.setTexture(&white_rook); break;
			case PieceUtils::White | PieceUtils::Queen: square.setTexture(&white_queen); break;
			case PieceUtils::White | PieceUtils::King: square.setTexture(&white_king); break;
			case PieceUtils::Black | PieceUtils::Pawn: square.setTexture(&black_pawn); break;
			case PieceUtils::Black | PieceUtils::Knight: square.setTexture(&black_knight); break;
			case PieceUtils::Black | PieceUtils::Bishop: square.setTexture(&black_bishop); break;
			case PieceUtils::Black | PieceUtils::Rook: square.setTexture(&black_rook); break;
			case PieceUtils::Black | PieceUtils::Queen: square.setTexture(&black_queen); break;
			case PieceUtils::Black | PieceUtils::King: square.setTexture(&black_king); break;
			case PieceUtils::Blocker: square.setTexture(&duck); break;
			default:
				continue;
			}
			m_pieces->push_back(square);
		}
		game->state_changed = false;
	}
	if (m_size_or_position_changed) {
		float promSize = m_size / 4;
		m_promotion_bg.setPosition(m_pos);
		m_promotion_bg.setSize(sf::Vector2f{m_size, promSize});
		for (int i = 0; i < m_promotions.size(); i++) {
			auto& prom = m_promotions[i];
			prom.setPosition(sf::Vector2f{ m_pos.x + promSize * i, m_pos.y });
			prom.setSize(sf::Vector2f{ promSize, promSize });
		}
		m_size_or_position_changed = false;
	}
}


HandlerCheckReoults BoardView::HandlerCheck(int x, int y, sf::RenderTarget& target) {
	auto localPos = target.mapPixelToCoords(sf::Vector2i{ x,y });
	localPos -= m_board.getPosition();
	return {
		(localPos.x >= 0 && localPos.y >= 0 && localPos.x <= m_size && localPos.y <= m_size),
		localPos
	};
}

void BoardView::HandleMouseRealease(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target)
{
	auto check = HandlerCheck(e.x, e.y, target);
	if (!check.is_ok) return;
}

void BoardView::HandleMousePress(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target)
{
	auto check = HandlerCheck(e.x, e.y, target);
	if (!check.is_ok) return;
	int squareSize = static_cast<int>(m_size / 8);
	HumanPlayer* toMove = nullptr;
	HumanPlayer* to_move_blocker = nullptr;
	if (auto d = dynamic_cast<HumanPlayer*>(white.get()); d != nullptr)
	{
		if (d->move) toMove = d;
		if (d->blocker_move) to_move_blocker = d;
	}
	if (auto d = dynamic_cast<HumanPlayer*>(black.get()); d != nullptr)
	{
		if (d->move) toMove = d;
		if (d->blocker_move) to_move_blocker = d;
	}
	int newSelected = ((static_cast<int>(check.local_position.y) / squareSize) << 3) | (static_cast<int>(check.local_position.x) / squareSize);
	int selectorSize = static_cast<int>(m_size / 4);
	if (toMove) {
		if (m_promotion_move != NullMove && check.local_position.y < selectorSize) {
			PieceType selector = static_cast<PieceType>(check.local_position.x / selectorSize + 2);
			if (selector > Pawn && selector < King) {
				toMove->TryMove(m_promotion_move.from, m_promotion_move.to, selector);
				m_selected = -1;
				m_promotion_move = NullMove;
				m_move_selection_changed = true;
				return;
			}
		}
		else {
			m_promotion_move = NullMove;
			m_move_selection_changed = true;
		}
		if (m_selected == -1) {
			m_selected = newSelected;
			m_move_selection_changed = true;
		}
		else if (m_selected == newSelected) {
			m_selected = -1;
			m_move_selection_changed = true;
		}
		else {
			auto rs = toMove->TryMove(m_selected, newSelected);
			if (rs == TryMoveResoult::VALID) {
				m_selected = -1;
				m_move_selection_changed = true;
				return;
			}
			else if (rs == TryMoveResoult::PROMOTION) {
				m_promotion_move = { 0,static_cast<uint8_t>(m_selected),static_cast<uint8_t>(newSelected), 0 };
				m_move_selection_changed = true;
				return;
			}
			m_selected = newSelected;
			m_move_selection_changed = true;
		}
	}
	if (to_move_blocker) {
		if (to_move_blocker->TryMoveBlocker(newSelected)) {
			m_selected = -1;
			m_move_selection_changed = true;
		}else{
			m_selected = newSelected;
			m_move_selection_changed = true;
		}
	}

}

void BoardView::HandleMouseMove(sf::Event::MouseMoveEvent& e, sf::RenderTarget& target)
{
	auto check = HandlerCheck(e.x, e.y, target);
	if (!check.is_ok) return;
}

void BoardView::LoadTextures()
{
	if (m_textures_loaded) return;
	m_textures_loaded = true;
	if (!white_pawn.loadFromFile("Assets/Pieces/Chess.com/wp.png"))
		throw std::invalid_argument("White Pawn texture missing");
	if (!black_pawn.loadFromFile("Assets/Pieces/Chess.com/bp.png"))
		throw std::invalid_argument("Black Pawn texture missing");

	if (!white_knight.loadFromFile("Assets/Pieces/Chess.com/wn.png"))
		throw std::invalid_argument("White Knight texture missing");
	if (!black_knight.loadFromFile("Assets/Pieces/Chess.com/bn.png"))
		throw std::invalid_argument("Black Knight texture missing");

	if (!white_bishop.loadFromFile("Assets/Pieces/Chess.com/wb.png"))
		throw std::invalid_argument("White Bishop texture missing");
	if (!black_bishop.loadFromFile("Assets/Pieces/Chess.com/bb.png"))
		throw std::invalid_argument("Black Bishop texture missing");

	if (!white_rook.loadFromFile("Assets/Pieces/Chess.com/wr.png"))
		throw std::invalid_argument("White Rook texture missing");
	if (!black_rook.loadFromFile("Assets/Pieces/Chess.com/br.png"))
		throw std::invalid_argument("Black Rook texture missing");

	if (!white_queen.loadFromFile("Assets/Pieces/Chess.com/wq.png"))
		throw std::invalid_argument("White Queen texture missing");
	if (!black_queen.loadFromFile("Assets/Pieces/Chess.com/bq.png"))
		throw std::invalid_argument("Black Queen texture missing");

	if (!white_king.loadFromFile("Assets/Pieces/Chess.com/wk.png"))
		throw std::invalid_argument("White King texture missing");
	if (!black_king.loadFromFile("Assets/Pieces/Chess.com/bk.png"))
		throw std::invalid_argument("Black King texture missing");

	if (!duck.loadFromFile("Assets/Pieces/Chess.com/duck.png"))
		throw std::invalid_argument("Duck texture missing");
}

BoardView::~BoardView()
{

}

void BoardView::SetPosition(sf::Vector2f m_pos)
{
	this->m_pos = m_pos;
	this->m_board.setPosition(m_pos);
	m_size_or_position_changed = true;
}

sf::Vector2f BoardView::GetPosition()
{
	return m_pos;
}

void BoardView::SetSize(float m_size)
{
	this->m_size = m_size;
	this->m_board.setSize(sf::Vector2f{ m_size,m_size });
	m_size_or_position_changed = true;
}

float BoardView::GetSize()
{
	return m_size;
}


