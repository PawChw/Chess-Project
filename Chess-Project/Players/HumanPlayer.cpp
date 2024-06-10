#include "HumanPlayer.h"


PieceType HumanPlayer::triggerPromotion()
{
	return PieceType();
}

Move HumanPlayer::Think(Board bd) {
	std::unique_lock lk(m);
	board = bd;
	legalMoves = bd.GetLegalMoves();
	toMake = NullMove;
	toMoveBlocker = -1;
	std::cout << "Available moves";
	for (auto& mv : legalMoves) {
		auto fromFile = GetFile(mv.from), fromRank = GetRank(mv.from);
		auto toFile = GetFile(mv.to), toRank = GetRank(mv.to);
		std::cout << '\n' << static_cast<char>('a'+fromFile) << static_cast<char>('1' + fromRank) << '\t' << static_cast<char>('a' + toFile) << static_cast<char>('1' + toRank);
	}
	std::cout <<  std::endl;
	move = true;
	cv.wait(lk, [this] {return toMake != NullMove; });
	legalMoves.clear();
	lk.unlock();
	cv.notify_all();
	return toMake;
}

bool HumanPlayer::TryMove(Square from, Square to) {
	if (!move) return false;
	Move candidate = { 0 };
	candidate.movedPiece = board.getPieceOnSquare(from);
	candidate.from = from;
	candidate.to = to;
	int index;
	if (getPieceType(candidate.movedPiece) == Pawn && GetRank(to) == (getColor(candidate.movedPiece) == White ? 7 : 0)) {
		candidate.promotedToPieceType = Queen;
	}
	index = findMove(legalMoves, candidate);
	if (index == -1) return false;
	{
		std::unique_lock lk(m);
		toMake = legalMoves.at(index);
		move = false;
	}
	cv.notify_all();
	return true;
}

Square HumanPlayer::ThinkBlocker(Board bd) {
	std::unique_lock lk(m);
	board = bd;
	toMake = NullMove;
	if(!legalMoves.empty())
		legalMoves.clear();
	toMoveBlocker = -1;
	notAvailableSquares = bd.getAllPiecesBitboard();
	BitboardHelpers::setBit(notAvailableSquares, bd.blockerSquare);
	Bitboard av = ~notAvailableSquares;
	std::cout << "Available blocks";
	while (av) {
		int index = BitboardHelpers::getAndClearIndexOfLSB(av);
		auto toFile = GetFile(index), toRank = GetRank(index);
		std::cout << '\n' << static_cast<char>('a' + toFile) << static_cast<char>('1' + toRank);
	}
	std::cout << std::endl;
	blockerMove = true;
	cv.wait(lk, [this] {return toMoveBlocker != -1; });
	lk.unlock();
	cv.notify_all();
	return toMoveBlocker;
}

bool HumanPlayer::TryMoveBlocker(Square to) {
	if (!blockerMove) return false;
	if (BitboardHelpers::getBit(notAvailableSquares, to)) return false;
	{
		std::unique_lock lk(m);
		toMoveBlocker = to;
		blockerMove = false;
	}
	cv.notify_all();
	return true;
}