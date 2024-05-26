#pragma once
#include <cstdint>
#include <stdexcept>

static class BitboardHelpers
{
public:
	static void SetSquare(uint64_t& bitboard, uint8_t index);
	static void ClearSquare(uint64_t& bitboard, uint8_t index);
	static void ToggleSquare(uint64_t& bitboard, uint8_t index);
	static bool SquareIsSet(uint64_t& bitboard, uint8_t index);
	static uint8_t ClearAndGetIngexOfLSB(uint64_t& bitboard);
};

