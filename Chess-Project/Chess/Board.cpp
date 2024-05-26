#include "Board.h"

Board::Board()
{
	this->whitePieces.at(2).setPosition(32);
}

std::array<PieceData, 64> Board::getBoardState() {
	std::array<PieceData, 64> pieces;
	pieces.fill({ PieceType::None , false});
	for (Piece piece : this->whitePieces) {
		pieces.at(piece.getPosition()) = { piece.type, true };
	}
	for (Piece piece : this->blackPieces) {
		pieces.at(piece.getPosition()) = { piece.type, false };
	}
	return pieces;
}
