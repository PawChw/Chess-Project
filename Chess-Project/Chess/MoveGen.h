#pragma once
#include <array>
#include "Piece.h"
#include "Square.h"
#include "BitboardHelpers.h"

static const std::array<Coords, 4> diagonal = { Coords{-1, 1}, Coords{1, 1}, Coords{1, -1}, Coords{-1, -1} };
static const std::array<Coords, 4> orthagonal = { Coords{0, 1}, Coords{1,0}, Coords{0, -1}, Coords{-1, 0} };
static const std::array<Coords, 8> knight = { Coords{2,1}, Coords{2,-1}, Coords{1,-2}, Coords{1,2}, Coords{-1,2}, Coords{-1,-2}, Coords{-2,-1}, Coords{-2,1} };


inline Bitboard GetKingMoves(Square square, Square enemyKing);
inline Bitboard GetQueenMoves(Square square, Bitboard blockers);
inline Bitboard GetRookMoves(Square square, Bitboard blockers);
inline Bitboard GetBishopMoves(Square square, Bitboard blockers);
inline Bitboard GetKnightMoves(Square square); 
inline Bitboard GetPawnMoves(Square square, Bitboard blockers, bool isWhite, Square epSquare, bool attacksOnly);
inline Bitboard GetPawnSilentMoves(Square square, Bitboard blockers, bool isWhite);
inline Bitboard GetPawnAttacks(Square square, bool isWhite);
Bitboard GetPieceMoves(PieceType pieceType, Square square, Square enemyKing, Bitboard blockers, bool isWhite, Square epSquare, bool attacksOnly);