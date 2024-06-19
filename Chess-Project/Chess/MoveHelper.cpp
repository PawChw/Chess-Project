#include "MoveHelper.h"


int CmpMoves(const Move& m1, const Move& m2)
{
    return std::memcmp(&m1, &m2, 4);
}

Move* quickNonCastleSearch(Move* begin, Move* end, Move toFind) {
    while (begin < end) {
        Move* half = begin + (end - begin) / 2;
        int diff = CmpMoves(toFind, *half);
        if (diff == 0) return half;
        else if (diff > 0) begin = half + 1;
        else end = half;
    }
    return nullptr;
}

Move* quickSearchCastle(Move* begin, Move* end, Move toFind) {
    if (CmpMoves(toFind, *(end - 1)) == 0) return end - 1;
    if (CmpMoves(toFind, *(end - 2)) == 0) return end - 2;
    return nullptr;
}

Move* quickSearch(Move* begin, Move* end, Move toFind) {
    if(PieceUtils::GetPieceType(toFind.moved_piece)==PieceUtils::King && std::abs(toFind.from - toFind.to)==2) return quickSearchCastle(begin, end, toFind);
    Move last = *(end - 1);
    while (PieceUtils::GetPieceType(last.moved_piece) == PieceUtils::King && std::abs(last.from - last.to) == 2) {
        end--;
        last = *(end - 1);
    }
    return quickNonCastleSearch(begin, end, toFind);
}

int FindMove(std::vector<Move> m_moves, Move toFind)
{
    Move* moves_ptr = m_moves.data();
    Move* resoult = quickSearch(moves_ptr, moves_ptr + m_moves.size(), toFind);
    if (resoult == nullptr) return -1;
    return static_cast<int>(resoult - moves_ptr);
}

bool IsMoveInVector(std::vector<Move> m_moves, Move toFind)
{
    Move* moves_ptr = m_moves.data();
    Move* resoult = quickSearch(moves_ptr, moves_ptr + m_moves.size(), toFind);
    return resoult != nullptr;
}
