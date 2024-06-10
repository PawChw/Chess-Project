#pragma once
#include<array>
#include<vector>
#include<stdexcept>

using Bitboard = int64_t;

class BitboardHelpers
{
public:
    static inline bool getBit(Bitboard& bb, unsigned int square) {
        auto result = static_cast<bool>(bb & (1ull << square));
        return result;
    }

    static inline void setBit(Bitboard& bb, unsigned int square) {
        bb |= 1ull << square;
    }

    static inline void toggleBit(Bitboard& bb, unsigned int square) {
        bb ^= 1ull << square;
    }

    static inline void clearBit(Bitboard& bb, unsigned int square) {
        bb &= ~(1ull << square);
    }

    static inline unsigned int getIndexOfLSB(Bitboard& bb) {
        if (!bb) throw std::invalid_argument("Board is empty");
        unsigned long index;
        _BitScanForward64(&index, bb);
        return static_cast<unsigned int>(index);
    }

    static inline unsigned int getAndClearIndexOfLSB(Bitboard& bb) {
        if (!bb) throw std::invalid_argument("Board is empty");
        unsigned long index;
        _BitScanForward64(&index, bb);
        clearBit(bb, index);
        return static_cast<unsigned int>(index);
    }

    static inline std::vector<unsigned int> GetAllBits(Bitboard bb) {
        std::vector<unsigned int> bits;
        for (int i = 0; i < 64; i++) {
            if (static_cast<bool>(getBit(bb, i)))
                bits.push_back(i);
        }
        return bits;
    }

    static inline unsigned int GetNumOfBitsSet(Bitboard bb) {
        unsigned int bits = 0;
        for (int i = 0; i < 64; i++) {
            if (getBit(bb, i))
                bits++;
        }
        return bits;
    }

};

