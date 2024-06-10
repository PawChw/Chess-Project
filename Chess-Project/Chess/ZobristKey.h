#pragma once
#include<array>
#include<cstdint>
#include<random>
#include<chrono>
#include<time.h>
#include "BitboardHelpers.h"

using Zobrist = uint64_t;
class ZobristKey
{
	static std::once_flag initFlag;
	static std::array<std::array<Zobrist, 64>, 12> hashes;
	static Zobrist blackToMove;
	static void InitOnce();
public:
	static void Init();

	static Zobrist Compute(std::array<Bitboard, 12> bitboards, bool isBlacksTurn);
};

