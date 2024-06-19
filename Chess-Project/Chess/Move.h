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

	bool Get(int color, int side) const {
		uint8_t mask = 0b1;
		if (side) mask <<= 1;
		if (color) mask <<= 2;
		return static_cast<bool>(mask & rights);
	}

	void Set(int color, int side, bool _val) {
		uint8_t mask = 0b1;
		if (side) mask <<= 1;
		if (color) mask <<= 2;
		if (_val) rights |= mask;
		else rights &= ~mask;
	}

	void Set(int color, std::array<bool, 2> _val) {
		Set(color, 0, _val[0]);
		Set(color, 1, _val[1]);
	}

	void Set(std::array< std::array<bool, 2>, 2> _val) {
		Set(0, _val[0]);
		Set(1, _val[1]);
	}

	bool operator==(const CastleRights& other) const {
		return other.rights == rights;
	}
};

struct CastleMoves {
private:
	uint8_t m_moves;
public:
	CastleMoves() : m_moves(0) {}
	CastleMoves(std::array < bool, 2> _rights) : m_moves(0) {
		if (_rights[0]) m_moves |= 0b1;
		if (_rights[0]) m_moves |= 0b10;
	}

	bool Get(int side) const {
		uint8_t mask = 0b1;
		if (side) mask <<= 1;
		return static_cast<bool>(mask & m_moves);
	}

	void Set(int side, bool _val) {
		uint8_t mask = 0b1;
		if (side) mask <<= 1;
		if (_val) m_moves |= mask;
		else m_moves &= ~mask;
	}

	void Set(std::array<bool, 2> _val) {
		Set(0, _val[0]);
		Set(1, _val[1]);
	}

	bool operator==(const CastleMoves& other) const {
		return other.m_moves == m_moves;
	}

	bool toBool() const {
		return static_cast<bool>(m_moves);
	}
};

#pragma pack(push, 1) // Ensure the struct is tightly packed
struct Move {
	uint8_t moved_piece;
	uint8_t from;
	uint8_t to;
	uint8_t promoted_to_piece_type;
	Square ep_square;
	Piece captured_piece;
	CastleRights castle_rights_lost; // k, q, K, Q
	CastleMoves is_castle;

	bool operator==(const Move& other) const {
		return to == other.to && from == other.from && moved_piece == other.moved_piece && captured_piece == other.captured_piece && promoted_to_piece_type == other.promoted_to_piece_type && ep_square == other.ep_square && is_castle == other.is_castle && castle_rights_lost == other.castle_rights_lost;
	}

	bool operator!=(const Move& other) const {
		return !(*this == other);
	}
};
#pragma pack(pop)

static const Move NullMove = { 0,0,0,0,0,0,CastleRights(),CastleMoves() };