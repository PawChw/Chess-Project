#pragma once
#include "../BoardView.h"
class Board :
    public BoardView
{
private:
public:
    std::array<PieceType, 64> getBoardState();
};

