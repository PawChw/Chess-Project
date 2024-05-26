#include "BitboardHelpers.h"

void BitboardHelpers::SetSquare(uint64_t& bitboard, uint8_t index)
{
	if (index >= 64) throw std::invalid_argument("Index out of range");
	bitboard |= 1ll << index;
}

void BitboardHelpers::ClearSquare(uint64_t& bitboard, uint8_t index)
{
	if (index >= 64) throw std::invalid_argument("Index out of range");
	bitboard |= ~(1ll << index);
}

void BitboardHelpers::ToggleSquare(uint64_t& bitboard, uint8_t index)
{
	if (index >= 64) throw std::invalid_argument("Index out of range");
	bitboard ^= 1ll << index;
}

bool BitboardHelpers::SquareIsSet(uint64_t& bitboard, uint8_t index)
{
	if (index >= 64) throw std::invalid_argument("Index out of range");
	return ((bitboard >> index) & 1) != 0;
}

uint8_t BitboardHelpers::ClearAndGetIngexOfLSB(uint64_t& bitboard)
{
	uint8_t index = 63;
	while (((bitboard >> index) & 1 && index > 0) != 1)
		index++;
	ClearSquare(bitboard, index);
	return index;
}
