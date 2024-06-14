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

template <typename T, int S>
bool compareArrays(const std::array<T, S>& arr1, const std::array<T, S>& arr2) {
	return std::equal(arr1.begin(), arr1.end(), arr2.begin());
}

template <typename T, int S>
bool compareArrays(const T* arr1, const T* arr2) {
	return std::equal(arr1, arr1+S, arr2);
}


using namespace PieceUtils;
class Board {
private:
	std::vector<Move> legalMovesCache;
	std::vector<Move> gameMoveHistory;
	std::vector<Zobrist> gameHistory;
	Zobrist currHash = 0;
	std::vector<unsigned int> historicHalfMoves;
	unsigned int halfMoveClock = 0;
	unsigned int fullMoveClock = 0;
	Piece board[64] = {0};
	void ParseFEN(std::string FEN);
public:
	Board();
	Board(std::string toParse);
	Board(const Board& other) = default;
	bool isWhiteToMove = true;
	int ply_count = 0;
	Square epSquare = -1,
		whiteKing = 4,
		blackKing = 60,
		blockerSquare = -1;
	std::array<std::array<bool, 2>, 2 > castleRights = { {{true, true}, {true, true}} }; // k, q, K, Q
	const std::vector<Move>& GetLegalMoves();
	const std::vector<Move> GetCaptures();
	void MakeMove(Move move);
	void MoveBlocker(Square newBlockerSquare);
	void ForceMakeMove(Move move);
	void UndoMove();
	Bitboard getPieceAttacks(Piece piece, Square square) const;
	Bitboard getBitboard(PieceType pieceType) const;
	Bitboard getBitboard(PieceType pieceType, Color color) const;
	Bitboard getWhiteBitboard() const;
	Bitboard getBlackBitboard() const;
	Bitboard getAllPiecesBitboard() const;
	Piece getPieceOnSquare(Square square) const;
	bool isSquareAttacked(Square square, bool byWho) const;
	bool isInsufficientMaterial();
	bool is50MoveRule() const;
	bool isSteelMate();
	bool isRepetition();
	bool isDraw();
	bool isInCheck(bool whoIs) const;
	bool isCheckMate();
	bool wasProomtion() const;
	Zobrist getZobristKey() const;
	std::array<Piece, 64> GetBoard() const;

	bool operator==(const Board& other) {
		auto rs = ply_count == other.ply_count;
		rs &= compareArrays<Piece, 64>(board, other.board);
		rs &= epSquare == other.epSquare;
		rs &= compareArrays(castleRights.at(1), other.castleRights.at(1));
		rs &= compareArrays(castleRights.at(0), other.castleRights.at(0));
		rs &= blockerSquare == blockerSquare;
		rs &= whiteKing == other.whiteKing;
		rs &= blackKing == other.blackKing;
		rs &= isWhiteToMove == other.isWhiteToMove;
		rs &= halfMoveClock == other.halfMoveClock;
		return rs;
	}

	bool operator!=(const Board& other) {
		auto rs = (*this == other);
		return !rs;
	}
};