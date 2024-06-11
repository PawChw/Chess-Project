#pragma once
#include <cstdint>
#include <cmath>
#include <thread>
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
	std::array<MoveEval, mask + 1>* transposition;
	uint8_t max_depth;
	int NegaMax(Board& bd, int alpha, int beta, int depth);
	int Eval(Board& bd) const;
	static const int Checkmate = 10000;
	int PieceVals[King+1] = { 0, 100, 300, 330, 500, 1000, Checkmate };

    int mgIncrement[7] = { 0,0,1,1,2,4,0 };

    int mg_table[12][64];
    int eg_table[12][64];

public:
	ComputerPlayer(uint8_t max_depth = 2);
	Square ThinkBlocker(Board bd) override;
	Move Think(Board bd) override;
	~ComputerPlayer();
};