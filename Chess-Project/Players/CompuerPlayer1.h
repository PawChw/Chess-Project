#pragma once
#include <cstdint>
#include <cmath>
#include <thread>
#include <array>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <chrono>
#include "../Game.h"
#include "../Chess.h"
#include "../Players/CompuerPlayer.h"

class ComputerPlayer1 : public IBlockerPlayer {
private:
	static const Zobrist mask = GB1;
	std::unique_ptr< std::array<MoveEval, mask + 1>> transposition = std::make_unique< std::array<MoveEval, mask + 1>>();
    const uint8_t max_depth;
	static const int Checkmate = 10000;
	const std::array<int, King+1> PieceVals = { 0, 100, 325, 325, 500, 975, Checkmate };
	Zobrist curr=0;
    int mgIncrement[7] = { 0,0,1,1,2,4,0 };
    const Clock maxTime;
    const int qseDepth;

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
    //threading specific
    std::mutex alpha_mutex, beta_mutex, ev_mutex, best_move_mutex;
    std::unordered_map<Zobrist, std::recursive_mutex> evaluatedPositions;
    std::atomic_int moveIndex;
    void NegaMaxThread(Board bd, int& alpha, int& beta, int depth, const std::vector<Move>& legalMoves, Move& bestMove, transpositionFlag& bestFlag, int& bestValue);
    int NegaMax(Board& bd, int alpha, int beta, int depth);
    int NegaScout(Board& bd, int alpha, int beta, int depth);
    int quiesce(Board& bd, int alpha, int beta, int checks = 0);
    std::vector<Move> filterSilentMoves(Board& bd, std::vector<Move> moves);

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
    ComputerPlayer1(uint8_t max_depth = 5, Clock maxTime = std::chrono::seconds(20000), int qseDepth = 3);
	Square ThinkBlocker(Board bd) override;
	Move Think(Board bd) override;
	~ComputerPlayer1();
};