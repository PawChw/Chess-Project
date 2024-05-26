#include "Board.h"

std::array<PieceType, 64> Board::getBoardState() {
	std::array<PieceType, 64> pieces;
	pieces.fill(PieceType::None);
	return pieces;
}