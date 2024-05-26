#pragma once
#include<array>

enum class PieceType {
	None = 0, Pawn, Knight, Bishop, Rook, Queen, King
};

struct PieceData {
	PieceType pieceType;
	bool isWhite;
};

class BoardView {
public:
	virtual std::array<PieceData, 64> getBoardState() = 0;
};