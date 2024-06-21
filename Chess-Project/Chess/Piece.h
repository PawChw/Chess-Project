#pragma once
#include<cstdint>

using Color = uint8_t;
using PieceType = uint8_t;
using Piece = uint8_t;

namespace PieceUtils {
	const static Color White = 8; //		01000
	const static Color Black = 16; //		10000

	const static PieceType None = 0; //		00000
	const static PieceType Pawn = 1;//		00001
	const static PieceType Knight = 2;//	00010
	const static PieceType Bishop = 3;//	00011
	const static PieceType Rook = 4;//		00100
	const static PieceType Queen = 5;//		00101
	const static PieceType King = 6;//		00110
	const static PieceType Blocker = 7;//	00111


	Color inline GetColor(Piece piece) noexcept {
		return (piece & 24);
	}
	PieceType inline GetPieceType(Piece piece) noexcept {
		return (piece & 7);
	}

	Piece inline ChangePieceTypeIfNotNone(Piece piece, PieceType pieceType) {
		if (!(pieceType & 7)) return piece;
		return (piece & 24) | (pieceType & 7);
	}

	Piece inline ChangeColor(Piece piece) {
		return piece ^ (White|Black);
	}
};