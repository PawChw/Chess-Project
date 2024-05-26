#include "PieceView.h"

PieceView::PieceView(const PieceData& piece, const sf::Vector2f& pos, int squareSize): piece(piece)
{
	this->rect.setSize(sf::Vector2f(squareSize, squareSize));
	this->rect.setPosition(pos);
	sf::Texture pieceTexture;
	switch (piece.pieceType) {
	case PieceType::Pawn:
		if (piece.isWhite) {
			if (!pieceTexture.loadFromFile("Assets/Pieces/Default/wp.png"))
				throw std::invalid_argument("White Pawn texture missing");
		}
		else {
			if (!pieceTexture.loadFromFile("Assets/Pieces/Default/bp.png"))
				throw std::invalid_argument("Black Pawn texture missing");
		}
		this->rect.setTexture(&pieceTexture);
		break;
	case PieceType::Knight:
		if (piece.isWhite) {
			if (!pieceTexture.loadFromFile("Assets/Pieces/Default/wn.png"))
				throw std::invalid_argument("White Knight texture missing");
		}
		else {
			if (!pieceTexture.loadFromFile("Assets/Pieces/Default/bn.png"))
				throw std::invalid_argument("Black Knight texture missing");
		}
		this->rect.setTexture(&pieceTexture);
		break;
	case PieceType::Bishop:
		if (piece.isWhite) {
			if (!pieceTexture.loadFromFile("Assets/Pieces/Default/wb.png"))
				throw std::invalid_argument("White Bishop texture missing");
		}
		else {
			if (!pieceTexture.loadFromFile("Assets/Pieces/Default/bb.png"))
				throw std::invalid_argument("Black Bishop texture missing");
		}
		this->rect.setTexture(&pieceTexture);
		break;
	case PieceType::Rook:
		if (piece.isWhite) {
			if (!pieceTexture.loadFromFile("Assets/Pieces/Default/wr.png"))
				throw std::invalid_argument("White Rook texture missing");
		}
		else {
			if (!pieceTexture.loadFromFile("Assets/Pieces/Default/br.png"))
				throw std::invalid_argument("Black Rook texture missing");
		}
		this->rect.setTexture(&pieceTexture);
		break;
	case PieceType::Queen:
		if (piece.isWhite) {
			if (!pieceTexture.loadFromFile("Assets/Pieces/Default/wq.png"))
				throw std::invalid_argument("White Queen texture missing");
		}
		else {
			if (!pieceTexture.loadFromFile("Assets/Pieces/Default/bq.png"))
				throw std::invalid_argument("Black Queen texture missing");
		}
		this->rect.setTexture(&pieceTexture);
		break;
	case PieceType::King:
		if (piece.isWhite) {
			if (!pieceTexture.loadFromFile("Assets/Pieces/Default/wk.png"))
				throw std::invalid_argument("White King texture missing");
		}
		else {
			if (!pieceTexture.loadFromFile("Assets/Pieces/Default/bk.png"))
				throw std::invalid_argument("Black King texture missing");
		}
		this->rect.setTexture(&pieceTexture);
		break;
	default:
		this->rect.setTexture(NULL);
		break;
	}
}

void PieceView::setPosition(sf::Vector2f& pos)
{
	this->rect.setPosition(pos);
}

const sf::RectangleShape& PieceView::getDrawable()
{
	return this->rect;
}
