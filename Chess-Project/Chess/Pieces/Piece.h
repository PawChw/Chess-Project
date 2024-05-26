#pragma once
#include "../BoardView.h"
#include <stdexcept>

class Piece {
private:
	uint8_t position;
public:
	const PieceType type;
	const bool isWhite;
	Piece(uint8_t position, PieceType type = PieceType::Pawn, bool isWhite = true) : type(type), position(position), isWhite(isWhite) {
		if (type == PieceType::None) throw std::invalid_argument("Can't create non piece");
		if (position > 63) throw std::invalid_argument("Piece not on board");
	}
	Piece(uint8_t position, bool isWhite) : type(PieceType::Pawn), position(position), isWhite(isWhite) {
		if (position > 63) throw std::invalid_argument("Piece not on board");
	}

	uint8_t getPosition() {
		return this->position;
	}

	void setPosition(uint8_t position) {
		if (position > 63) throw std::invalid_argument("Piece not on board");
		this->position = position;
	}
};