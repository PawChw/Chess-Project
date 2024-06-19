#pragma once
#include <cstdint>
#include <cmath>
#include <thread>
#include <array>
#include <mutex>
#include <queue>
#include <map>
#include <chrono>
#include "../Game.h"
#include "../Chess.h"
#include "../Chess/MoveGen.h"

enum class TranspositionFlag : int8_t {
    QUIESCE = -1, NONE = 0, LOWERBOUND, EXACT, UPPERBOUND
};

struct MoveEval {
	int8_t depth;
	Move move;
	int eval;
	Zobrist hash;
    TranspositionFlag flag;
};

using Clock = std::chrono::system_clock::duration;

enum TranspositionSize : Zobrist {
    MB256 = 0x7FFFFF, MB512 = 0xFFFFFF, GB1 = 0x1FFFFFF, GB2 = 0x3FFFFFF, GB4 = 0x3FFFFFF, GB8 = 0x7FFFFFF, GB16 = 0xFFFFFFF
    // keeping tt size as even number makes it easy to size limit thanks to bitwise or instead of division by size
};

class ComputerPlayer : public IBlockerPlayer {
private:
	static const Zobrist mask = GB1;
	std::unique_ptr< std::array<MoveEval, mask + 1>> transposition = std::make_unique< std::array<MoveEval, mask + 1>>();
    const uint8_t max_depth;
	static const int checkmate = 10000;
	const std::array<int, King+7> piece_vals = { 0, 100, 300, 310, 470, 940, 0, 125, 295, 320, 530, 940, 0 };
	Zobrist curr=0;
    int mg_increment[7] = { 0,0,1,1,2,4,0 };
    int atk_vals[7] = { 0,1,2,2,3,5,0 };
    const Clock desired_time;
    const int qseDepth;

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

    int mg_table[15][64];
    int eg_table[15][64];
    int SafetyTable[62] = {
            0,  0,   1,   2,   3,   5,   7,   9,  12,  15,
          18,  22,  26,  30,  35,  39,  44,  50,  56,  62,
          68,  75,  82,  85,  89,  97, 105, 113, 122, 131,
         140, 150, 169, 180, 191, 202, 213, 225, 237, 248,
         260, 272, 283, 295, 307, 319, 330, 342, 354, 366,
         377, 389, 401, 412, 424, 436, 448, 459, 471, 483,
         494, 500
    };

    int NegaMax(Board& bd, int alpha, int beta, int depth);
    int NegaScout(Board& bd, int alpha, int beta, int depth);
    int Quiesce(Board& bd, int alpha, int beta, int checks = 0);
    static inline int Clamp(int min, int max, int val);
    Bitboard GetKingSafety(bool side, Board& bd) const;
    std::vector<Move> FilterSilentMoves(Board& bd, std::vector<Move> m_moves);

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
    int Eval(Board& bd) const;
	ComputerPlayer(uint8_t maxDepth = 8, Clock maxTime = std::chrono::seconds(5), int qseDepth = 3);
	Square ThinkBlocker(Board bd) override;
	Move Think(Board bd) override;
	~ComputerPlayer();
};