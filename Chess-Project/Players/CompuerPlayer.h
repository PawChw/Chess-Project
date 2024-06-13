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

using Clock = std::chrono::system_clock::duration;

class ComputerPlayer : public IBlockerPlayer {
private:
	static const Zobrist mask = 0xFFFFFF;
	std::unique_ptr< std::array<MoveEval, mask + 1>> transposition = std::make_unique< std::array<MoveEval, mask + 1>>();
	uint8_t max_depth;
	int NegaMax(Board& bd, int alpha, int beta, int depth);
	static const int Checkmate = 10000;
	std::array<int, King+1> PieceVals = { 0, 100, 325, 325, 500, 975, Checkmate };
	Zobrist curr=0;
    int mgIncrement[7] = { 0,0,1,1,2,4,0 };
    Clock maxTime;

    const int16_t* mg_pesto_table[7] = {
        nullptr,
        mg_pawn_table,
        mg_knight_table,
        mg_bishop_table,
        mg_rook_table,
        mg_queen_table,
        mg_king_table
    };

    const int16_t* eg_pesto_table[7] = {
        nullptr,
        eg_pawn_table,
        eg_knight_table,
        eg_bishop_table,
        eg_rook_table,
        eg_queen_table,
        eg_king_table
    };

    int16_t mg_table[(PieceUtils::King | PieceUtils::White) + 1][64];
    int16_t eg_table[(PieceUtils::King | PieceUtils::White) + 1][64];

public:
    const static int16_t mg_pawn_table[64];
    const static int16_t eg_pawn_table[64];
    const static int16_t mg_knight_table[64];
    const static int16_t eg_knight_table[64];
    const static int16_t mg_bishop_table[64];
    const static int16_t eg_bishop_table[64];
    const static int16_t mg_rook_table[64];
    const static int16_t eg_rook_table[64];
    const static int16_t mg_queen_table[64];
    const static int16_t eg_queen_table[64];
    const static int16_t mg_king_table[64];
    const static int16_t eg_king_table[64];
    int Eval(Board& bd) const;
	ComputerPlayer(uint8_t max_depth = 7, Clock maxTime = std::chrono::seconds(2));
	Square ThinkBlocker(Board bd) override;
	Move Think(Board bd) override;
	~ComputerPlayer();
};