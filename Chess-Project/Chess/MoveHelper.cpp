#include "MoveHelper.h"


int cmpMoves(const Move& m1, const Move& m2)
{
    return std::memcmp(&m1, &m2, 4);
}

Move* quickNonCastleSearch(Move* begin, Move* end, Move toFind) {
    if (end <= begin) return nullptr;
    Move* half = begin + (end - begin) / 2;
    int diff = cmpMoves(toFind, *half);
    if (diff == 0) return half;
    else if (diff > 0) return quickNonCastleSearch(half + 1, end, toFind);
    return quickNonCastleSearch(begin, half, toFind);
}

Move* quickSearchCastle(Move* begin, Move* end, Move toFind) {
    if (cmpMoves(toFind, *(end - 1)) == 0) return end - 1;
    if (cmpMoves(toFind, *(end - 2)) == 0) return end - 2;
    return nullptr;
}

Move* quickSearch(Move* begin, Move* end, Move toFind) {
    if(PieceUtils::getPieceType(toFind.movedPiece)==PieceUtils::King && std::abs(toFind.from - toFind.to)) return quickSearchCastle(begin, end, toFind);
    return quickNonCastleSearch(begin, end, toFind);
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
