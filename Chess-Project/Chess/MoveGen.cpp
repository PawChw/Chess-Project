#include "MoveGen.h"


inline Bitboard GetKingMoves(Square square, Square enemyKing)
{
	Bitboard possibleMoves = 0ull;
	Coords startPos = Coords::fromSquareIndex(square);
	Coords temp;
	for (auto& dir : orthagonal) {
		temp = startPos + dir;
		if (temp.isValid())
			BitboardHelpers::setBit(possibleMoves, temp.toSquareIndex());
	}
	for (auto& dir : diagonal) {
		temp = startPos + dir;
		if (temp.isValid())
			BitboardHelpers::setBit(possibleMoves, temp.toSquareIndex());
	}
	startPos = Coords::fromSquareIndex(enemyKing);
	for (auto& dir : orthagonal) {
		temp = startPos + dir;
		if (temp.isValid())
			BitboardHelpers::clearBit(possibleMoves, temp.toSquareIndex());
	}
	for (auto& dir : diagonal) {
		temp = startPos + dir;
		if (temp.isValid())
			BitboardHelpers::clearBit(possibleMoves, temp.toSquareIndex());
	}
	return possibleMoves;
}

Bitboard GetQueenMoves(Square square, Bitboard blockers)
{
	return GetBishopMoves(square, blockers) | GetRookMoves(square, blockers);
}

Bitboard GetRookMoves(Square square, Bitboard blockers)
{
	Bitboard possibleMoves = 0ull;
	Coords startPos = Coords::fromSquareIndex(square);
	Coords temp;
	for (auto& dir : orthagonal) {
		temp = startPos + dir;
		while (temp.isValid()) {
			BitboardHelpers::setBit(possibleMoves, temp.toSquareIndex());
			if (BitboardHelpers::getBit(blockers, temp.toSquareIndex())) break;
			temp += dir;
		}
	}
	return possibleMoves;
}

Bitboard GetBishopMoves(Square square, Bitboard blockers)
{
	Bitboard possibleMoves = 0ull;
	Coords startPos = Coords::fromSquareIndex(square);
	Coords temp;
	for (auto& dir : diagonal) {
		temp = startPos + dir;
		while (temp.isValid()) {
			BitboardHelpers::setBit(possibleMoves, temp.toSquareIndex());
			if (BitboardHelpers::getBit(blockers, temp.toSquareIndex())) break;
			temp += dir;
		}
	}
	return possibleMoves;
}

Bitboard GetKnightMoves(Square square)
{
	Bitboard possibleMoves = 0ull;
	Coords startPos = Coords::fromSquareIndex(square);
	Coords temp;
	for (auto& dir : knight) {
		temp = startPos + dir;
		if (temp.isValid()) {
			BitboardHelpers::setBit(possibleMoves, temp.toSquareIndex());
		}
	}
	return possibleMoves;
}

Bitboard GetPawnMoves(Square square, Bitboard blockers, bool isWhite, Square epSquare, bool attacksOnly)
{
	Bitboard bb = GetPawnAttacks(square, isWhite);
	if (!attacksOnly) {
		if (isValidSquare(epSquare))
			blockers |= 1ull << epSquare;
		bb &= blockers;
		bb |= GetPawnSilentMoves(square, blockers, isWhite);
	}
	return bb;
}

Bitboard GetPawnSilentMoves(Square square, Bitboard blockers, bool isWhite)
{
	Bitboard possibleMoves = 0ull;
	int8_t direction = isWhite ? -1 : 1;
	Coords startPos = Coords::fromSquareIndex(square);
	Coords temp = startPos;
	temp.rank += direction;
	if (temp.isValid() && !BitboardHelpers::getBit(blockers, temp.toSquareIndex())) {
		BitboardHelpers::setBit(possibleMoves, temp.toSquareIndex());
		if (startPos.rank == (isWhite ? 6 : 1)) {
			temp.rank += direction;
			if (temp.isValid() && !BitboardHelpers::getBit(blockers, temp.toSquareIndex()))
				BitboardHelpers::setBit(possibleMoves, temp.toSquareIndex());
		}
	}

	return possibleMoves;
}

Bitboard GetPawnAttacks(Square square, bool isWhite)
{
	Bitboard possibleAttacks = 0ull;
	int8_t direction = isWhite ? -1 : 1;
	Coords startPos = Coords::fromSquareIndex(square);
	Coords temp = startPos;
	temp.rank += direction;
	temp.file += 1;
	if (temp.isValid())
		BitboardHelpers::setBit(possibleAttacks, temp.toSquareIndex());
	temp.file -= 2;
	if (temp.isValid())
		BitboardHelpers::setBit(possibleAttacks, temp.toSquareIndex());
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

