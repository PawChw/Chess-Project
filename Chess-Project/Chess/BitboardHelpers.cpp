#include "BitboardHelpers.h"

bool BitboardHelpers::GetBit(const Bitboard& bb, unsigned int square) {
    return static_cast<bool>((bb >> square) & 1);
}

void BitboardHelpers::SetBit(Bitboard& bb, unsigned int square) {
    bb |= 1ull << square;
}

void BitboardHelpers::ToggleBit(Bitboard& bb, unsigned int square) {
    bb ^= 1ull << square;
}

void BitboardHelpers::ClearBit(Bitboard& bb, unsigned int square) {
    bb &= ~(1ull << square);
}

unsigned int BitboardHelpers::GetIndexOfLSB(const Bitboard& bb) {
    if (!bb) throw std::invalid_argument("Board is empty");
    unsigned long index;
    _BitScanForward64(&index, bb);
    return static_cast<unsigned int>(index);
}

unsigned int BitboardHelpers::GetAndClearIndexOfLSB(Bitboard& bb) {
    if (!bb) throw std::invalid_argument("Board is empty");
    unsigned long index;
    _BitScanForward64(&index, bb);
    ClearBit(bb, index);
    return static_cast<unsigned int>(index);
}

unsigned int BitboardHelpers::GetIndexOfFSB(const Bitboard& bb) {
    if (!bb) throw std::invalid_argument("Board is empty");
    unsigned long index;
    _BitScanReverse64(&index, bb);
    return static_cast<unsigned int>(index);
}

unsigned int BitboardHelpers::GetAndClearIndexOfFSB(Bitboard& bb) {
    if (!bb) throw std::invalid_argument("Board is empty");
    unsigned long index;
    _BitScanReverse64(&index, bb);
    ClearBit(bb, index);
    return static_cast<unsigned int>(index);
}

std::vector<unsigned int> BitboardHelpers::GetAllBits(Bitboard bb) {
    std::vector<unsigned int> bits;
    for (int i = 0; i < 64; i++) {
        if (static_cast<bool>(GetBit(bb, i)))
            bits.push_back(i);
    }
    return bits;
}

unsigned int BitboardHelpers::GetNumOfBitsSet(Bitboard bb) {
    return __popcnt64(bb);
}
