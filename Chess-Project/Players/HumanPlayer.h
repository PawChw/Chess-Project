#pragma once
#include <condition_variable>
#include <atomic>

#include "../Game.h"
#include "../Chess.h"

class HumanPlayer :
    public IBlockerPlayer
{
private:
    std::mutex m;
    std::condition_variable cv;
    Move toMake = NullMove;
    Square toMoveBlocker = -1;
    Bitboard notAvailableSquares;
    PieceType triggerPromotion();
    std::vector<Move> legalMoves;
    Board board;
public:
    std::atomic_bool move = false, blockerMove = false;
    HumanPlayer() = default;
    Move Think(Board bd) override;
    bool TryMove(Square from, Square to);
    Square ThinkBlocker(Board bd) override;
    bool TryMoveBlocker(Square to);
};

