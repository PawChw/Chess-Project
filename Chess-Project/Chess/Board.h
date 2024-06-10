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


using namespace PieceUtils;
class Board {
private:
	std::vector<Move> legalMovesCache;
	std::vector<Move> gameMoveHistory;
	std::vector<Zobrist> gameHistory;
	std::vector<unsigned int> historicHalfMoves;
	unsigned int halfMoveClock = 0;
	unsigned int fullMoveClock = 0;
	std::array<Piece, 64> board = { 0 };
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
	Zobrist getZobristKey();
	const std::array<Piece, 64>& GetBoard();

	bool operator==(const Board& other) {
		return ply_count == other.ply_count && compareArrays(board, other.board) && epSquare == other.epSquare && castleRights.at(0).at(0) == castleRights.at(0).at(0) && castleRights.at(1).at(1) == castleRights.at(1).at(1) && castleRights.at(0).at(1) == castleRights.at(0).at(1) && castleRights.at(1).at(0) == castleRights.at(1).at(0) && blockerSquare == blockerSquare && whiteKing == other.whiteKing && blackKing == other.blackKing && isWhiteToMove == other.isWhiteToMove && halfMoveClock == other.halfMoveClock;
	}

	bool operator!=(const Board& other) {
		return !(*this == other);
	}
};