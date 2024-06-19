#include "ZobristKey.h"

std::once_flag ZobristKey::m_init_flag;
std::array<std::array<Zobrist, 64>, 12> ZobristKey::hashes;
Zobrist ZobristKey::black_to_move;

void ZobristKey::InitOnce() {
	std::mt19937_64 rng(static_cast<std::mt19937_64::result_type>(
		std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<Zobrist> dist;
	for (auto& pieceArray : hashes) {
		for (auto& square : pieceArray) {
			square = dist(rng);
		}
	}
	black_to_move = dist(rng);
}

void ZobristKey::Init() {
	std::call_once(m_init_flag, InitOnce);
}

Zobrist ZobristKey::Compute(std::array<Bitboard, 12> bitboards, bool isBlacksTurn) {
	Zobrist value = isBlacksTurn ? black_to_move : 0ull;
	for (int i = 0; i < 12; i++) {
		Bitboard bb = bitboards[i];
		while (static_cast<bool>(bb)) {
			value ^= hashes[i][BitboardHelpers::GetAndClearIndexOfLSB(bb)];
		}
	}
	return value;
}

Zobrist ZobristKey::GetMove(Zobrist old, Square from, Square to, Piece piece, bool changeMover)
{
	Zobrist newZobrist = old;
	Move(newZobrist, from, to, piece, changeMover);
	return newZobrist;
}

void ZobristKey::Move(Zobrist& old, Square from, Square to, Piece piece, bool changeMover)
{
	int pieceIndex = PieceUtils::GetPieceType(piece) - 1;
	if (PieceUtils::GetColor(piece) == PieceUtils::Black) pieceIndex += 6;
	if (changeMover) old ^= black_to_move;
	old ^= hashes[pieceIndex][from];
	old ^= hashes[pieceIndex][to];
}
