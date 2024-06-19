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
    std::condition_variable m_cv;
    Move m_to_move = NullMove;
    Square to_move_blocker = -1;
    Bitboard m_not_available_squares = 0;
    std::vector<Move> m_legal_moves;
    Board m_board;
public:
    std::mutex m;
    std::atomic_bool move = false, blocker_move = false;
    HumanPlayer() = default;
    Move Think(Board bd) override;
    Square ThinkBlocker(Board bd) override;
    TryMoveResoult TryMove(Square from, Square to, PieceType promotedTo = None);
    bool TryMoveBlocker(Square to);
    bool TryMove();
    bool TryMoveBlocker();
};

