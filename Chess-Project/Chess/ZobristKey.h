#pragma once
#include<array>
#include<cstdint>
#include<random>
#include<chrono>
#include<time.h>
#include "BitboardHelpers.h"
#include "Square.h"
#include "Piece.h"

using Zobrist = uint64_t;
class ZobristKey
{
	static std::once_flag m_init_flag;
	static void InitOnce();
public:
	static void Init();
	static std::array<std::array<Zobrist, 64>, 12> hashes;
	static Zobrist black_to_move;

	static Zobrist Compute(std::array<Bitboard, 12> bitboards, bool isBlacksTurn);
	static Zobrist GetMove(Zobrist old, Square from, Square to, Piece piece, bool changeMover = true);
	static void Move(Zobrist& old, Square from, Square to, Piece piece, bool changeMover = true);
};

