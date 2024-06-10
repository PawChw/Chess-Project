#include "MoveHelper.h"


int cmpMoves(const Move& m1, const Move& m2)
{
    return std::memcmp(&m1, &m2, 4);
}

Move* quickSearch(Move* begin, Move* end, Move toFind) {
    if (end <= begin) return nullptr;
    Move* half = begin + (end - begin) / 2;
    int diff = cmpMoves(toFind, *half);
    if (diff == 0) return half;
    else if (diff > 0) return quickSearch(half + 1, end, toFind);
    return quickSearch(begin, half, toFind);
}

int findMove(std::vector<Move> moves, Move toFind)
{
    Move* moves_ptr = moves.data();
    Move* resoult = quickSearch(moves_ptr, moves_ptr + moves.size(), toFind);
    if (resoult == nullptr) return -1;
    return static_cast<int>(resoult - moves_ptr);
}

bool isMoveInVector(std::vector<Move> moves, Move toFind)
{
    Move* moves_ptr = moves.data();
    Move* resoult = quickSearch(moves_ptr, moves_ptr + moves.size(), toFind);
    return resoult != nullptr;
}
