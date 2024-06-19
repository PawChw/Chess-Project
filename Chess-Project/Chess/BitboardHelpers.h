#pragma once
#include<array>
#include<vector>
#include<stdexcept>

using Bitboard = uint64_t;

class BitboardHelpers
{
public:
    static inline bool GetBit(Bitboard& bb, unsigned int square) {
        auto result = static_cast<bool>(bb & (1ull << square));
        return result;
    }

    static inline void SetBit(Bitboard& bb, unsigned int square) {
        bb |= 1ull << square;
    }

    static inline void ToggleBit(Bitboard& bb, unsigned int square) {
        bb ^= 1ull << square;
    }

    static inline void ClearBit(Bitboard& bb, unsigned int square) {
        bb &= ~(1ull << square);
    }

    static inline unsigned int GetIndexOfLSB(const Bitboard& bb) {
        if (!bb) throw std::invalid_argument("Board is empty");
        unsigned long index;
        _BitScanForward64(&index, bb);
        return static_cast<unsigned int>(index);
    }

    static inline unsigned int GetAndClearIndexOfLSB(Bitboard& bb) {
        if (!bb) throw std::invalid_argument("Board is empty");
        unsigned long index;
        _BitScanForward64(&index, bb);
        ClearBit(bb, index);
        return static_cast<unsigned int>(index);
    }

    static inline unsigned int GetIndexOfFSB(const Bitboard& bb) {
        if (!bb) throw std::invalid_argument("Board is empty");
        unsigned long index;
        _BitScanReverse64(&index, bb);
        return static_cast<unsigned int>(index);
    }

    static inline unsigned int GetAndClearIndexOfFSB(Bitboard& bb) {
        if (!bb) throw std::invalid_argument("Board is empty");
        unsigned long index;
        _BitScanReverse64(&index, bb);
        ClearBit(bb, index);
        return static_cast<unsigned int>(index);
    }

    static inline std::vector<unsigned int> GetAllBits(Bitboard bb) {
        std::vector<unsigned int> bits;
        for (int i = 0; i < 64; i++) {
            if (static_cast<bool>(GetBit(bb, i)))
                bits.push_back(i);
        }
        return bits;
    }

    static inline unsigned int GetNumOfBitsSet(Bitboard bb) {
        unsigned int bits = 0;
        for (int i = 0; i < 64; i++) {
            if (GetBit(bb, i))
                bits++;
        }
        return bits;
    }

};

