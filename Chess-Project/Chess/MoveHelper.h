#pragma once
#include<vector>
#include<cstdint>
#include<cstring>

#include "Move.h"
#include "Piece.h"


int CmpMoves(const Move& m1, const Move& m2); /* REQUIREMENT
		Moves are in order!
		movedPiece asc
		from asc
		to asc
		promotedToPieceType asc(for pawns only)

		@ret + if m1 > m2
		@ret 0 if m1 == m2
		@ret - if m1 < m2
*/

int FindMove(std::vector<Move> m_moves, Move toFind);/* REQUIREMENT
		Moves are in order!
		movedPiece asc
		from asc
		to asc
		promotedToPieceType asc(for pawns only)
*/

bool IsMoveInVector(std::vector<Move> m_moves, Move toFind);/* REQUIREMENT
		Moves are in order!
		movedPiece asc
		from asc
		to asc
		promotedToPieceType asc(for pawns only)
*/