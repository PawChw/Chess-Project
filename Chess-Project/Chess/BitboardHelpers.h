#pragma once

#include <cstdint>
#include <vector>
#include <stdexcept>
#include <intrin.h>

using Bitboard = uint64_t;

class BitboardHelpers {
public:
    static bool GetBit(const Bitboard& bb, unsigned int square);
    static void SetBit(Bitboard& bb, unsigned int square);
    static void ToggleBit(Bitboard& bb, unsigned int square);
    static void ClearBit(Bitboard& bb, unsigned int square);
    static unsigned int GetIndexOfLSB(const Bitboard& bb);
    static unsigned int GetAndClearIndexOfLSB(Bitboard& bb);
    static unsigned int GetIndexOfFSB(const Bitboard& bb);
    static unsigned int GetAndClearIndexOfFSB(Bitboard& bb);
    static std::vector<unsigned int> GetAllBits(Bitboard bb);
    static unsigned int GetNumOfBitsSet(Bitboard bb);
};
