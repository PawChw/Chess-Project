#pragma once
#include <array>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <memory>

#include "Piece.h"
#include "BitboardHelpers.h"
#include "MoveGen.h"
#include "Move.h"
#include "Parser.h"
#include "ZobristKey.h"
#include "MoveHelper.h"

template <typename T, int S> //std::array<int,s>
bool CompareArrays(const std::array<T, S>& arr1, const std::array<T, S>& arr2) {
	return std::equal(arr1.begin(), arr1.end(), arr2.begin());
}

template <typename T, int S> //int[s]
bool CompareArrays(const T* arr1, const T* arr2) {
	return std::equal(arr1, arr1+S, arr2);
}


using namespace PieceUtils;
class Board {
private:
	std::vector<Move> m_legal_moves_cache;
	std::vector<Move> m_game_move_history;
	std::vector<Zobrist> m_game_history;
	Zobrist m_curr_hash = 0;
	std::vector<unsigned int> m_historic_half_moves;
	unsigned int m_half_move_clock = 0;
	unsigned int m_full_move_clock = 0;
	Bitboard m_board[12] = {0};
	void ParseFEN(std::string FEN);
	static inline int PieceToBitboardIndex(Piece piece);
public:
	Board();
	Board(std::string to_parse);
	Board(const Board& other) = default;
	bool is_white_to_move = true;
	int ply_count = 0;
	Square ep_square = -1,
		white_king = 4,
		black_king = 60,
		blocker_square = -1;
	std::array<std::array<bool, 2>, 2 > castle_rights = { {{true, true}, {true, true}} }; // k, q, K, Q
	const std::vector<Move>& GetLegalMoves();
	void MakeMove(Move move);
	void MoveBlocker(Square newBlockerSquare);
	void ForceMakeMove(Move move);
	void UndoMove();
	Bitboard GetPieceAttacks(Piece piece, Square square) const;
	Bitboard GetBitboard(PieceType pieceType) const;
	Bitboard GetBitboard(PieceType pieceType, Color color) const;
	Bitboard GetWhiteBitboard() const;
	Bitboard GetBlackBitboard() const;
	Bitboard GetAllPiecesBitboard() const;
	Piece GetPieceOnSquare(Square square) const;
	bool IsSquareAttacked(Square square, bool byWho) const;
	bool IsInsufficientMaterial();
	bool Is50MoveRule() const;
	bool IsSteelMate();
	bool IsRepetition();
	bool IsDraw();
	bool IsInCheck(bool whoIs) const;
	bool IsCheckMate();
	bool IsKingCapturd() const;
	bool WasProomtion() const;
	Zobrist GetZobristKey() const;
	std::array<Piece, 64> GetBoard() const;

	bool operator==(const Board& other) {
		auto rs = ply_count == other.ply_count;
		rs &= CompareArrays<Bitboard, 12>(m_board, other.m_board);
		rs &= ep_square == other.ep_square;
		rs &= CompareArrays(castle_rights.at(1), other.castle_rights.at(1));
		rs &= CompareArrays(castle_rights.at(0), other.castle_rights.at(0));
		rs &= blocker_square == blocker_square;
		rs &= white_king == other.white_king;
		rs &= black_king == other.black_king;
		rs &= is_white_to_move == other.is_white_to_move;
		rs &= m_half_move_clock == other.m_half_move_clock;
		return rs;
	}

	bool operator!=(const Board& other) {
		auto rs = (*this == other);
		return !rs;
	}
};