#pragma once
#include<vector>
#include<cstdint>
#include<cstring>

#include "Move.h"
#include "Piece.h"


int cmpMoves(const Move& m1, const Move& m2); /* REQUIREMENT
		Moves are in order!
		movedPiece asc
		from asc
		to asc
		promotedToPieceType asc(for pawns only)

		@ret + if m1 > m2
		@ret 0 if m1 == m2
		@ret - if m1 < m2
*/

int findMove(std::vector<Move> moves, Move toFind);/* REQUIREMENT
		Moves are in order!
		movedPiece asc
		from asc
		to asc
		promotedToPieceType asc(for pawns only)
*/

bool isMoveInVector(std::vector<Move> moves, Move toFind);/* REQUIREMENT
		Moves are in order!
		movedPiece asc
		from asc
		to asc
		promotedToPieceType asc(for pawns only)
*/