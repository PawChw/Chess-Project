#pragma once
#include <condition_variable>
#include <atomic>

#include "../Game.h"
#include "../Chess.h"

class HumanPlayer :
    public IBlockerPlayer
{
private:
    std::condition_variable cv;
    Move toMake = NullMove;
    Square toMoveBlocker = -1;
    Bitboard notAvailableSquares = 0;
    PieceType triggerPromotion();
    std::vector<Move> legalMoves;
    Board board;
public:
    std::mutex m;
    std::atomic_bool move = false, blockerMove = false;
    HumanPlayer() = default;
    Move Think(Board bd) override;
    Square ThinkBlocker(Board bd) override;
    bool TryMove(Square from, Square to);
    bool TryMoveBlocker(Square to);
    bool TryMove();
    bool TryMoveBlocker();
};

