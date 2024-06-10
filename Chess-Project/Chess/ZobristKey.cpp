#include "ZobristKey.h"

std::once_flag ZobristKey::initFlag;
std::array<std::array<Zobrist, 64>, 12> ZobristKey::hashes;
Zobrist ZobristKey::blackToMove;

void ZobristKey::InitOnce() {
	std::mt19937_64 rng(static_cast<std::mt19937_64::result_type>(
		std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<Zobrist> dist;
	for (auto& pieceArray : hashes) {
		for (auto& square : pieceArray) {
			square = dist(rng);
		}
	}
	blackToMove = dist(rng);
}

void ZobristKey::Init() {
	std::call_once(initFlag, InitOnce);
}

Zobrist ZobristKey::Compute(std::array<Bitboard, 12> bitboards, bool isBlacksTurn) {
	Zobrist value = isBlacksTurn ? blackToMove : 0ull;
	for (int i = 0; i < 12; i++) {
		Bitboard bb = bitboards[i];
		while (static_cast<bool>(bb)) {
			value ^= hashes[i][BitboardHelpers::getAndClearIndexOfLSB(bb)];
		}
	}
	return value;
}