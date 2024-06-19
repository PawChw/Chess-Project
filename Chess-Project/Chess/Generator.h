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
	const Board& m_board;
	bool m_moves_for;
	std::array<std::array<Bitboard, King + 1>, 2> bitboards = { 0 }; // all, pawn, knight... for colors(1 is white 0 is black due to bool conversion)
	Bitboard m_all_pieces_bitboard;
	void SeedBitboards();
	bool CheckMove(Move& move) const;
public:
	Generator() = delete;
	Generator(const Board& m_board);
	Generator(const Board& m_board, bool m_moves_for);
	std::vector<Move> GenerateLegalMoves(bool capturesOnly = false); /*
		Moves are in order!
		movedPiece asc
		from asc
		to asc
		promotedToPieceType asc(for pawns only)
		*/
};

