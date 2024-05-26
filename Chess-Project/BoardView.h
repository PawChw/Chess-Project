#pragma once
#include<array>

enum class PieceType {
	None = 0, Pawn, Knight, Bishop, Rook, Queen, King
};

class BoardView {
public:
	virtual std::array<PieceType, 64> getBoardState() = 0;
};