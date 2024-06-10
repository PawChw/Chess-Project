#pragma once
#include <memory>

#include "../Game.h"
#include "../Chess.h"

class HumanPlayer :
    public IBlockerPlayer
{
private:
    Move toMake;
    PieceType triggerPromotion();
    std::shared_ptr<const std::vector<Move>> legalMoves;
public:
    Move Think(Board bd) override;
    bool TryMove();
};

