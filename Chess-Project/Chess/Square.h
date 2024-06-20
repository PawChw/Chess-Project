#pragma once

using Square = int8_t;
using File = uint8_t;
using Rank = uint8_t;

inline File GetFile(Square square) noexcept {
	return square % 8;
}

inline Rank GetRank(Square square) noexcept {
	return square / 8;
}

inline bool IsValidSquare(Square square) noexcept {
	return square < 64 && square >= 0;
}

inline bool IsValidSquare(File file, Rank rank) noexcept {
	return (file < 8 && rank < 8);
}

inline Square GetSquare(File file, Rank rank) noexcept {
	return rank * 8 + file;
}
