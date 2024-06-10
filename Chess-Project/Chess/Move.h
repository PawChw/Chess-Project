#pragma once
#include "Piece.h"
#include "Square.h"

#include <stdexcept>
#include <array>

struct CastleRights {
private:
	uint8_t rights;
public:
	CastleRights() : rights(0) {}
	CastleRights(std::array<std::array < bool, 2>, 2> _rights) : rights(0) {
		if (_rights[0][0]) rights |= 0b1;
		if (_rights[0][1]) rights |= 0b10;
		if (_rights[1][0]) rights |= 0b100;
		if (_rights[1][1]) rights |= 0b1000;
	}

	bool get(int color, int side) const {
		uint8_t mask = 0b1;
		if (side) mask <<= 1;
		if (color) mask <<= 2;
		return static_cast<bool>(mask & rights);
	}

	void set(int color, int side, bool _val) {
		uint8_t mask = 0b1;
		if (side) mask <<= 1;
		if (color) mask <<= 2;
		if (_val) rights |= mask;
		else rights &= ~mask;
	}

	void set(int color, std::array<bool, 2> _val) {
		set(color, 0, _val[0]);
		set(color, 1, _val[1]);
	}

	void set(std::array< std::array<bool, 2>, 2> _val) {
		set(0, _val[0]);
		set(1, _val[1]);
	}

	bool operator==(const CastleRights& other) const {
		return other.rights == rights;
	}
};

struct CastleMoves {
private:
	uint8_t moves;
public:
	CastleMoves() : moves(0) {}
	CastleMoves(std::array < bool, 2> _rights) : moves(0) {
		if (_rights[0]) moves |= 0b1;
		if (_rights[0]) moves |= 0b10;
	}

	bool get(int side) const {
		uint8_t mask = 0b1;
		if (side) mask <<= 1;
		return static_cast<bool>(mask & moves);
	}

	void set(int side, bool _val) {
		uint8_t mask = 0b1;
		if (side) mask <<= 1;
		if (_val) moves |= mask;
		else moves &= ~mask;
	}

	void set(std::array<bool, 2> _val) {
		set(0, _val[0]);
		set(1, _val[1]);
	}

	bool operator==(const CastleMoves& other) const {
		return other.moves == moves;
	}
};

#pragma pack(push, 1) // Ensure the struct is tightly packed
struct Move {
	uint8_t movedPiece;
	uint8_t from;
	uint8_t to;
	uint8_t promotedToPieceType;
	Square epSquare;
	Piece capturedPiece;
	CastleRights castleRightsLost; // k, q, K, Q
	CastleMoves isCastle;

	bool operator==(const Move& other) const {
		return to == other.to && from == other.from && movedPiece == other.movedPiece && capturedPiece == other.capturedPiece && promotedToPieceType == other.promotedToPieceType && epSquare == other.epSquare && isCastle == other.isCastle && castleRightsLost == other.castleRightsLost;
	}
};
#pragma pack(pop)