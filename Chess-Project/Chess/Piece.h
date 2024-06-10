#pragma once
#include<cstdint>

using Color = uint8_t;
using PieceType = uint8_t;
using Piece = uint8_t;

namespace PieceUtils {
	const static Color White = 8;
	const static Color Black = 16;

	const static PieceType None = 0;
	const static PieceType Pawn = 1;
	const static PieceType Knight = 2;
	const static PieceType Bishop = 3;
	const static PieceType Rook = 4;
	const static PieceType Queen = 5;
	const static PieceType King = 6;


	Color inline getColor(Piece piece) noexcept {
		return (piece & 24);
	}
	PieceType inline getPieceType(Piece piece) noexcept {
		return (piece & 7);
	}

	Piece inline changePieceTypeIfNotNone(Piece piece, PieceType pieceType) {
		if (!(pieceType & 7)) return piece;
		return (piece & 24) | (pieceType & 7);
	}
};