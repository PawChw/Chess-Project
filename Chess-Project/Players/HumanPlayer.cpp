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
	if (getPieceType(candidate.movedPiece) == Pawn && GetRank(to) == (getColor(candidate.movedPiece) == White ? 0 : 7)) {
		candidate.promotedToPieceType = Queen;
	}
	if (candidate.to == board.epSquare && getPieceType(candidate.movedPiece) == Pawn) candidate.capturedPiece = Pawn;
	else candidate.capturedPiece = board.getPieceOnSquare(to);
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

bool HumanPlayer::TryMove()
{
	if (!move) return false;
	{
		std::unique_lock lk(m);
		toMake = legalMoves.at(0);
		move = false;
	}
	cv.notify_all();
	return true;
}

bool HumanPlayer::TryMoveBlocker()
{
	if (!blockerMove) return false;
	auto to = BitboardHelpers::getIndexOfFSB(~notAvailableSquares);
	{
		std::unique_lock lk(m);
		toMoveBlocker = to;
		blockerMove = false;
	}
	cv.notify_all();
	return true;
}
