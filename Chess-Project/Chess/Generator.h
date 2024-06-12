#pragma once
#include <array>
#include <vector>

#include "Move.h"
#include "MoveGen.h"
#include "Piece.h"
#include "Square.h"
#include "Board.h"

class Generator
{
private:
	const Board& board;
	bool movesFor;
	std::array<std::array<Bitboard, King + 1>, 2> bitboards = { 0 }; // all, pawn, knight... for colors(1 is white 0 is black due to bool conversion)
	Bitboard allPiecesBitboard;
	void seedBitboards();
	bool checkMove(Move& move) const;
public:
	Generator() = delete;
	Generator(const Board& board);
	Generator(const Board& board, bool movesFor);
	std::vector<Move> GenerateLegalMoves(); /*
		Moves are in order!
		movedPiece asc
		from asc
		to asc
		promotedToPieceType asc(for pawns only)
		*/
};

