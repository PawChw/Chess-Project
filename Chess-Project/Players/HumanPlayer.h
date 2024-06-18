#pragma once
#include <condition_variable>
#include <atomic>

#include "../Game.h"
#include "../Chess.h"

enum class TryMoveResoult {
    INVALID = 0, VALID, PROMOTION
};

class HumanPlayer :
    public IBlockerPlayer
{
private:
    std::condition_variable cv;
    Move toMake = NullMove;
    Square toMoveBlocker = -1;
    Bitboard notAvailableSquares = 0;
    std::vector<Move> legalMoves;
    Board board;
public:
    PieceType promotionPiece = Queen;
    std::mutex m;
    std::atomic_bool move = false, blockerMove = false;
    HumanPlayer() = default;
    Move Think(Board bd) override;
    Square ThinkBlocker(Board bd) override;
    TryMoveResoult TryMove(Square from, Square to, PieceType promotedTo = None);
    bool TryMoveBlocker(Square to);
    bool TryMove();
    bool TryMoveBlocker();
};

