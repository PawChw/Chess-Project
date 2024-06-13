#include "BoardView.h"

std::atomic_bool BoardView::texturesLoaded = false;
sf::Texture BoardView::whitePawn;
sf::Texture BoardView::blackPawn;
sf::Texture BoardView::whiteKnight;
sf::Texture BoardView::blackKnight;
sf::Texture BoardView::whiteBishop;
sf::Texture BoardView::blackBishop;
sf::Texture BoardView::whiteRook;
sf::Texture BoardView::blackRook;
sf::Texture BoardView::whiteQueen;
sf::Texture BoardView::blackQueen;
sf::Texture BoardView::whiteKing;
sf::Texture BoardView::blackKing;
sf::Texture BoardView::duck;

void BoardView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(board);
	for (auto shape : *pieces) {
		target.draw(shape);
	}
	for (auto shape : *moves) {
		target.draw(shape);
	}
}

BoardView::BoardView(sf::Vector2f pos, float size, Players white, Players black, bool withBlockers) : size(size), pos(pos) {
	loadTextures();
	board = sf::RectangleShape(sf::Vector2f{ size ,size });
	boardTexture.loadFromFile("Assets/board.png");
	board.setPosition(pos);
	board.setTexture(&boardTexture);
	if (white == Players::HumanPlayer)
		this->white = std::make_shared<HumanPlayer>();
	else
		this->white = std::make_shared<ComputerPlayer>();

	if (black == Players::HumanPlayer)
		this->black = std::make_shared<HumanPlayer>();
	else
		this->black = std::make_shared<ComputerPlayer>();
	if (withBlockers)
		game = std::make_unique<BlockerGame>(this->white, this->black);
	else
		game = std::make_unique<Game>(this->white, this->black);
	Update();

}

void BoardView::Update()
{
	float squareSize = size / 8.f;
	sf::RectangleShape square(sf::Vector2f{ squareSize, squareSize });
	float circleRadius = squareSize / 6;
	float circleOffset = squareSize/2 - circleRadius;
	sf::CircleShape circle(circleRadius);
	if (game->stateChanged || selectedChanged || sizeOrPosChanged) {
		if(!moves->empty())
			moves->clear();
		std::unordered_set<uint16_t> drawnMoves;
		HumanPlayer* toMove = nullptr;
		HumanPlayer* toMoveBlocker = nullptr;
		if (auto d = dynamic_cast<HumanPlayer*>(white.get()); d != nullptr)
		{
			if (d->move) toMove = d;
			if (d->blockerMove) toMoveBlocker = d;
		}
		if (auto d = dynamic_cast<HumanPlayer*>(black.get()); d != nullptr)
		{
			if (d->move) toMove = d;
			if (d->blockerMove) toMoveBlocker = d;
		}
		if (toMove && selected != -1) {
			auto& legal_moves = game->bd.GetLegalMoves();
			for (Move move : legal_moves) {
				if (move.from == selected) {
					if (!drawnMoves.insert((static_cast<uint16_t>(move.from) << 8) | move.to).second) continue;
					legalMoves.push_back({ move.from, move.to, static_cast<bool>(move.capturedPiece), static_cast<bool>(move.promotedToPieceType) });
					if (move.capturedPiece && move.promotedToPieceType)
						circle.setFillColor(sf::Color(150, 150, 100, 200));
					else if (move.capturedPiece)
						circle.setFillColor(sf::Color(150, 100, 100, 200));
					else if (move.promotedToPieceType)
						circle.setFillColor(sf::Color(100, 150, 100, 200));
					else if (move.isCastle.toBool())
						circle.setFillColor(sf::Color(200, 200, 200, 200));
					else
						circle.setFillColor(sf::Color(100, 100, 100, 200));
					circle.setPosition(sf::Vector2f{ (pos.x + GetFile(move.to) * squareSize) + circleOffset, (pos.y + GetRank(move.to) * squareSize) + circleOffset });
					moves->push_back(circle);
				}
			}
		}
		else if (toMoveBlocker) {
			Bitboard available = ~(game->bd.getAllPiecesBitboard());
			if(isValidSquare(game->bd.blockerSquare))
				BitboardHelpers::clearBit(available, game->bd.blockerSquare);
			Square sq;
			circle.setFillColor(sf::Color(100, 100, 150, 200));
			while (available) {
				sq = BitboardHelpers::getAndClearIndexOfLSB(available);
				circle.setPosition(sf::Vector2f{ (pos.x + GetFile(sq) * squareSize) + circleOffset, (pos.y + GetRank(sq) * squareSize) + circleOffset });
				moves->push_back(circle);
			}
		}
		selectedChanged = false;
	}
	if (game->stateChanged || sizeOrPosChanged) {
		game->stateChanged = true;
		if(!pieces->empty())
			pieces->clear();
		square.setFillColor(sf::Color::White);
		for (Square i = 0; i < 64; i++) {
			square.setPosition(sf::Vector2f{ pos.x + (GetFile(i) * squareSize), pos.y + (GetRank(i) * squareSize) });
			auto piece = game->bd.getPieceOnSquare(i);
			switch (piece)
			{
			case PieceUtils::White | PieceUtils::Pawn: square.setTexture(&whitePawn); break;
			case PieceUtils::White | PieceUtils::Knight: square.setTexture(&whiteKnight); break;
			case PieceUtils::White | PieceUtils::Bishop: square.setTexture(&whiteBishop); break;
			case PieceUtils::White | PieceUtils::Rook: square.setTexture(&whiteRook); break;
			case PieceUtils::White | PieceUtils::Queen: square.setTexture(&whiteQueen); break;
			case PieceUtils::White | PieceUtils::King: square.setTexture(&whiteKing); break;
			case PieceUtils::Black | PieceUtils::Pawn: square.setTexture(&blackPawn); break;
			case PieceUtils::Black | PieceUtils::Knight: square.setTexture(&blackKnight); break;
			case PieceUtils::Black | PieceUtils::Bishop: square.setTexture(&blackBishop); break;
			case PieceUtils::Black | PieceUtils::Rook: square.setTexture(&blackRook); break;
			case PieceUtils::Black | PieceUtils::Queen: square.setTexture(&blackQueen); break;
			case PieceUtils::Black | PieceUtils::King: square.setTexture(&blackKing); break;
			case PieceUtils::Blocker: square.setTexture(&duck); break;
			default:
				continue;
			}
			pieces->push_back(square);
		}
		game->stateChanged = false;
		sizeOrPosChanged = false;
	}
}


HandlerCheckReoults BoardView::HandlerCheck(int x, int y, sf::RenderTarget& target) {
	auto localPos = target.mapPixelToCoords(sf::Vector2i{ x,y });
	localPos -= board.getPosition();
	return {
		(localPos.x >= 0 && localPos.y >= 0 && localPos.x <= size && localPos.y <= size),
		localPos
	};
}

void BoardView::HandleMouseRealease(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target)
{
	auto check = HandlerCheck(e.x, e.y, target);
	if (!check.isOk) return;
}

void BoardView::HandleMousePress(sf::Event::MouseButtonEvent& e, sf::RenderTarget& target)
{
	auto check = HandlerCheck(e.x, e.y, target);
	if (!check.isOk) return;
	int squareSize = size / 8;
	int newSelected = ((e.y / squareSize) << 3) | (e.x / squareSize);
	HumanPlayer* toMove = nullptr;
	HumanPlayer* toMoveBlocker = nullptr;
	if (auto d = dynamic_cast<HumanPlayer*>(white.get()); d != nullptr)
	{
		if (d->move) toMove = d;
		if (d->blockerMove) toMoveBlocker = d;
	}
	if (auto d = dynamic_cast<HumanPlayer*>(black.get()); d != nullptr)
	{
		if (d->move) toMove = d;
		if (d->blockerMove) toMoveBlocker = d;
	}
	if (toMove) {
		if (selected == -1) {
			selected = newSelected;
			selectedChanged = true;
		}
		else if (selected == newSelected) {
			selected = -1;
			selectedChanged = true;
		}
		else {
			for (auto mv : legalMoves) {
				if (mv.to == newSelected && mv.from == selected) {
					toMove->TryMove(mv.from, mv.to);
					selected = -1;
					selectedChanged = true;
					return;
				}
			}
			selected = newSelected;
			selectedChanged = true;
		}
	}
	if (toMoveBlocker) {
		if (toMoveBlocker->TryMoveBlocker(newSelected)) {
			selected = -1;
			selectedChanged = true;
		}else{
			selected = newSelected;
			selectedChanged = true;
		}
	}

}

void BoardView::HandleMouseMove(sf::Event::MouseMoveEvent& e, sf::RenderTarget& target)
{
	auto check = HandlerCheck(e.x, e.y, target);
	if (!check.isOk) return;
}

void BoardView::loadTextures()
{
	if (texturesLoaded) return;
	texturesLoaded = true;
	if (!whitePawn.loadFromFile("Assets/Pieces/Chess.com/wp.png"))
		throw std::invalid_argument("White Pawn texture missing");
	if (!blackPawn.loadFromFile("Assets/Pieces/Chess.com/bp.png"))
		throw std::invalid_argument("Black Pawn texture missing");

	if (!whiteKnight.loadFromFile("Assets/Pieces/Chess.com/wn.png"))
		throw std::invalid_argument("White Knight texture missing");
	if (!blackKnight.loadFromFile("Assets/Pieces/Chess.com/bn.png"))
		throw std::invalid_argument("Black Knight texture missing");

	if (!whiteBishop.loadFromFile("Assets/Pieces/Chess.com/wb.png"))
		throw std::invalid_argument("White Bishop texture missing");
	if (!blackBishop.loadFromFile("Assets/Pieces/Chess.com/bb.png"))
		throw std::invalid_argument("Black Bishop texture missing");

	if (!whiteRook.loadFromFile("Assets/Pieces/Chess.com/wr.png"))
		throw std::invalid_argument("White Rook texture missing");
	if (!blackRook.loadFromFile("Assets/Pieces/Chess.com/br.png"))
		throw std::invalid_argument("Black Rook texture missing");

	if (!whiteQueen.loadFromFile("Assets/Pieces/Chess.com/wq.png"))
		throw std::invalid_argument("White Queen texture missing");
	if (!blackQueen.loadFromFile("Assets/Pieces/Chess.com/bq.png"))
		throw std::invalid_argument("Black Queen texture missing");

	if (!whiteKing.loadFromFile("Assets/Pieces/Chess.com/wk.png"))
		throw std::invalid_argument("White King texture missing");
	if (!blackKing.loadFromFile("Assets/Pieces/Chess.com/bk.png"))
		throw std::invalid_argument("Black King texture missing");

	if (!duck.loadFromFile("Assets/Pieces/Chess.com/duck.png"))
		throw std::invalid_argument("Duck texture missing");
}

BoardView::~BoardView()
{
}

void BoardView::setPosition(sf::Vector2f pos)
{
	this->pos = pos;
	this->board.setPosition(pos);
	sizeOrPosChanged = true;
}

void BoardView::setSize(float size)
{
	this->size = size;
	this->board.setSize(sf::Vector2f{ size,size });
	sizeOrPosChanged = true;
}


