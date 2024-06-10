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

inline bool isValidSquare(Square square) noexcept {
	return square < 64 && square >= 0;
}

inline bool isValidSquare(File file, Rank rank) noexcept {
	return (file < 8 && rank < 8);
}

inline Square getSquare(File file, Rank rank) noexcept {
	return rank * 8 + file;
}

struct Coords {
	int8_t file;
	int8_t rank;

	Coords operator+(const Coords& other) {
		return Coords{ static_cast<int8_t>(file + other.file), static_cast<int8_t>(rank + other.rank) };
	}


	Coords& operator+=(const Coords& other) {
		rank += other.rank;
		file += other.file;
		return *this;
	}

	Coords operator*(int other) {
		return Coords{ static_cast<int8_t>(file * other), static_cast<int8_t>(rank * other) };
	}

	Square toSquareIndex() const noexcept {
		return rank * 8 + file;
	}

	static Coords fromSquareIndex(Square square) {
		return Coords{ static_cast<int8_t>(GetFile(square)), static_cast<int8_t>(GetRank(square)) };
	}

	bool inline isValid() const noexcept {
		return (file < 8 && rank < 8 && file >= 0 && rank >= 0);
	}
};