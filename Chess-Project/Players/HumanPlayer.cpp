#include "HumanPlayer.h"


Move HumanPlayer::Think(Board bd) {
	std::unique_lock lk(m);
	m_board = bd;
	m_legal_moves = bd.GetLegalMoves();
	m_to_move = NullMove;
	to_move_blocker = -1;
	move = true;
	m_cv.wait(lk, [this] {return m_to_move != NullMove; });
	m_legal_moves.clear();
	lk.unlock();
	m_cv.notify_all();
	return m_to_move;
}

TryMoveResoult HumanPlayer::TryMove(Square from, Square to, PieceType promotedTo) {
	if (!move) return TryMoveResoult::INVALID;
	Move candidate = { 0 };
	candidate.moved_piece = m_board.GetPieceOnSquare(from);
	candidate.from = from;
	candidate.to = to;
	int index;
	if (GetPieceType(candidate.moved_piece) == Pawn && GetRank(to) == (GetColor(candidate.moved_piece) == White ? 0 : 7)) {
		if (!promotedTo) return TryMoveResoult::PROMOTION;
		candidate.promoted_to_piece_type = promotedTo;
	}
	if (candidate.to == m_board.ep_square && GetPieceType(candidate.moved_piece) == Pawn) candidate.captured_piece = Pawn;
	else candidate.captured_piece = m_board.GetPieceOnSquare(to);
	index = FindMove(m_legal_moves, candidate);
	if (index == -1) return TryMoveResoult::INVALID;
	{
		std::unique_lock lk(m);
		m_to_move = m_legal_moves.at(index);
		move = false;
	}
	m_cv.notify_all();
	return TryMoveResoult::VALID;
}

Square HumanPlayer::ThinkBlocker(Board bd) {
	std::unique_lock lk(m);
	m_board = bd;
	m_to_move = NullMove;
	if(!m_legal_moves.empty())
		m_legal_moves.clear();
	to_move_blocker = -1;
	m_not_available_squares = bd.GetAllPiecesBitboard();
	BitboardHelpers::SetBit(m_not_available_squares, bd.blocker_square);
	Bitboard av = ~m_not_available_squares;
	blocker_move = true;
	m_cv.wait(lk, [this] {return to_move_blocker != -1; });
	lk.unlock();
	m_cv.notify_all();
	return to_move_blocker;
}

bool HumanPlayer::TryMoveBlocker(Square to) {
	if (!blocker_move) return false;
	if (BitboardHelpers::GetBit(m_not_available_squares, to)) return false;
	{
		std::unique_lock lk(m);
		to_move_blocker = to;
		blocker_move = false;
	}
	m_cv.notify_all();
	return true;
}

bool HumanPlayer::TryMove()
{
	if (!move) return false;
	{
		std::unique_lock lk(m);
		m_to_move = m_legal_moves.at(0);
		move = false;
	}
	m_cv.notify_all();
	return true;
}

bool HumanPlayer::TryMoveBlocker()
{
	if (!blocker_move) return false;
	auto to = BitboardHelpers::GetIndexOfFSB(~m_not_available_squares);
	{
		std::unique_lock lk(m);
		to_move_blocker = to;
		blocker_move = false;
	}
	m_cv.notify_all();
	return true;
}
