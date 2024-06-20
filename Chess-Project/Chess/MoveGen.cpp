#include "MoveGen.h"


Bitboard GetKingMoves(Square square, Square enemyKing) {
	Bitboard possibleMoves = 0ull;

	// Calculate king moves
	for (int dir : ROOK_DIRECTIONS) {
		int targetSquare = square + dir;
		if (targetSquare >= 0 && targetSquare < 64 && !(square % 8 == 0 && (dir == -1 || dir == -9 || dir == 7)) && !((square + 1) % 8 == 0 && (dir == 1 || dir == 9 || dir == -7))) {
			BitboardHelpers::SetBit(possibleMoves, targetSquare);
		}
	}
	for (int dir : BISHOP_DIRECTIONS) {
		int targetSquare = square + dir;
		if (targetSquare >= 0 && targetSquare < 64 && !(square % 8 == 0 && (dir == -1 || dir == -9 || dir == 7)) && !((square + 1) % 8 == 0 && (dir == 1 || dir == 9 || dir == -7))) {
			BitboardHelpers::SetBit(possibleMoves, targetSquare);
		}
	}

	// Calculate enemy king influence and clear those squares from possible moves
	for (int dir : ROOK_DIRECTIONS) {
		int targetSquare = enemyKing + dir;
		if (targetSquare >= 0 && targetSquare < 64 && !(enemyKing % 8 == 0 && (dir == -1 || dir == -9 || dir == 7)) && !((enemyKing + 1) % 8 == 0 && (dir == 1 || dir == 9 || dir == -7))) {
			BitboardHelpers::ClearBit(possibleMoves, targetSquare);
		}
	}
	for (int dir : BISHOP_DIRECTIONS) {
		int targetSquare = enemyKing + dir;
		if (targetSquare >= 0 && targetSquare < 64 && !(enemyKing % 8 == 0 && (dir == -1 || dir == -9 || dir == 7)) && !((enemyKing + 1) % 8 == 0 && (dir == 1 || dir == 9 || dir == -7))) {
			BitboardHelpers::ClearBit(possibleMoves, targetSquare);
		}
	}

	return possibleMoves;
}

Bitboard GetQueenMoves(Square square, Bitboard blockers)
{
	return GetBishopMoves(square, blockers) | GetRookMoves(square, blockers);
}

Bitboard GetRookMoves(Square square, Bitboard blockers) {
	Bitboard possibleMoves = 0ull;

	for (int dir : ROOK_DIRECTIONS) {
		int targetSquare = square;
		while (true) {
			targetSquare += dir;
			// Check if the target square is still on the board and valid
			if (targetSquare < 0 || targetSquare >= 64 || (targetSquare % 8 == 0 && dir == 1) || ((targetSquare + 1) % 8 == 0 && dir == -1)) {
				break;
			}
			BitboardHelpers::SetBit(possibleMoves, targetSquare);
			if (BitboardHelpers::GetBit(blockers, targetSquare)) {
				break;
			}
		}
	}

	return possibleMoves;
}

Bitboard GetBishopMoves(Square square, Bitboard blockers) {
	Bitboard possibleMoves = 0ull;

	for (int dir : BISHOP_DIRECTIONS) {
		int targetSquare = square;
		while (true) {
			targetSquare += dir;
			// Check if the target square is still on the board and valid
			if (targetSquare < 0 || targetSquare >= 64 || (targetSquare % 8 == 0 && (dir == 9 || dir == -7)) || ((targetSquare + 1) % 8 == 0 && (dir == 7 || dir == -9))) {
				break;
			}
			BitboardHelpers::SetBit(possibleMoves, targetSquare);
			if (BitboardHelpers::GetBit(blockers, targetSquare)) {
				break;
			}
		}
	}

	return possibleMoves;
}

Bitboard GetKnightMoves(Square square)
{
	return KNIGHT_MOVES_MASK[square];
}

Bitboard GetPawnMoves(Square square, Bitboard blockers, bool isWhite, Square epSquare, bool attacksOnly)
{
	Bitboard bb = GetPawnAttacks(square, isWhite);
	if (!attacksOnly) {
		if (IsValidSquare(epSquare))
			blockers |= 1ull << epSquare;
		bb &= blockers;
		bb |= GetPawnSilentMoves(square, blockers, isWhite);
	}
	return bb;
}

Bitboard GetPawnSilentMoves(Square square, Bitboard blockers, bool isWhite) {
	Bitboard possibleMoves = 0ull;
	Bitboard pawn = (1ull << square);

	if (isWhite) {
		Bitboard singleMove = pawn >> 8;
		if (!(singleMove & blockers)) {
			possibleMoves |= singleMove;
			// Check for the initial double move
			if ((pawn & 0x00FF000000000000) != 0) { // If the pawn is on the second rank (from white's perspective)
				Bitboard doubleMove = singleMove >> 8;
				if (!(doubleMove & blockers)) {
					possibleMoves |= doubleMove;
				}
			}
		}
	}
	else {
		Bitboard singleMove = pawn << 8;
		if (!(singleMove & blockers)) {
			possibleMoves |= singleMove;
			// Check for the initial double move
			if ((pawn & 0x000000000000FF00) != 0) { // If the pawn is on the seventh rank (from black's perspective)
				Bitboard doubleMove = singleMove << 8;
				if (!(doubleMove & blockers)) {
					possibleMoves |= doubleMove;
				}
			}
		}
	}

	return possibleMoves;
}

Bitboard GetPawnAttacks(Square square, bool isWhite)
{
	Bitboard possibleAttacks = 0ull;
	Bitboard pawn = (1ull << square);

	if (isWhite) {
		if ((pawn & 0xFEFEFEFEFEFEFEFE) != 0) // Ensure it's not on the A-file
			possibleAttacks |= (pawn >> 9);
		if ((pawn & 0x7F7F7F7F7F7F7F7F) != 0) // Ensure it's not on the H-file
			possibleAttacks |= (pawn >> 7);
	}
	else {
		if ((pawn & 0xFEFEFEFEFEFEFEFE) != 0) // Ensure it's not on the A-file
			possibleAttacks |= (pawn << 7);
		if ((pawn & 0x7F7F7F7F7F7F7F7F) != 0) // Ensure it's not on the H-file
			possibleAttacks |= (pawn << 9);
	}

	return possibleAttacks;
}

Bitboard GetPieceMoves(PieceType pieceType, Square square, Square enemyKing, Bitboard blockers, bool isWhite, Square epSquare, bool attacksOnly)
{
	Bitboard mask = attacksOnly ? blockers : UINT64_MAX;
	switch (pieceType)
	{
	case PieceUtils::Pawn:
		mask &= GetPawnMoves(square, blockers, isWhite, epSquare, attacksOnly);
		break;
	case PieceUtils::Knight:
		mask &= GetKnightMoves(square);
		break;
	case PieceUtils::Bishop:
		mask &= GetBishopMoves(square, blockers);
		break;
	case PieceUtils::Rook:
		mask &= GetRookMoves(square, blockers);
		break;
	case PieceUtils::Queen:
		mask &= GetQueenMoves(square, blockers);
		break;
	case PieceUtils::King:
		mask &= GetKingMoves(square, enemyKing);
		break;
	default:
		return 0ull;
	}
	return mask;
}

