#pragma once
#include <cstdint>
#include <cmath>
#include <thread>
#include <array>
#include <mutex>
#include <queue>
#include <chrono>
#include "../Game.h"
#include "../Chess.h"

struct MoveEval {
	int8_t depth;
	Move move;
	int eval;
	Zobrist hash;
	int8_t flag;
};

class ComputerPlayer : public IBlockerPlayer {
private:
	static const Zobrist mask = 0xFFFFFF;
	std::unique_ptr< std::array<MoveEval, mask + 1>> transposition = std::make_unique< std::array<MoveEval, mask + 1>>();
	uint8_t max_depth;
	int NegaMax(Board& bd, int alpha, int beta, int depth);
	int Eval(Board& bd) const;
	static const int Checkmate = 10000;
	std::array<int, King+1> PieceVals = { 0, 100, 325, 325, 500, 975, Checkmate };
	Zobrist curr=0;
    int mgIncrement[7] = { 0,0,1,1,2,4,0 };

    const int* mg_pesto_table[7] = {
        nullptr,
        mg_pawn_table,
        mg_knight_table,
        mg_bishop_table,
        mg_rook_table,
        mg_queen_table,
        mg_king_table
    };

    const int* eg_pesto_table[7] = {
        nullptr,
        eg_pawn_table,
        eg_knight_table,
        eg_bishop_table,
        eg_rook_table,
        eg_queen_table,
        eg_king_table
    };

    int mg_table[(PieceUtils::King | PieceUtils::White) + 1][64];
    int eg_table[(PieceUtils::King | PieceUtils::White) + 1][64];

public:
    const static int mg_pawn_table[64];
    const static int eg_pawn_table[64];
    const static int mg_knight_table[64];
    const static int eg_knight_table[64];
    const static int mg_bishop_table[64];
    const static int eg_bishop_table[64];
    const static int mg_rook_table[64];
    const static int eg_rook_table[64];
    const static int mg_queen_table[64];
    const static int eg_queen_table[64];
    const static int mg_king_table[64];
    const static int eg_king_table[64];
	ComputerPlayer(uint8_t max_depth = 2);
	Square ThinkBlocker(Board bd) override;
	Move Think(Board bd) override;
	~ComputerPlayer();
};