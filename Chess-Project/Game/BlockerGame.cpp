#include "BlockerGame.h"

BlockerGame::BlockerGame(IBlockerPlayer& white, IBlockerPlayer& black) : bd(Board()), white(white),
black(black)
{
	isGameOn = false;
}

GameTerminalState BlockerGame::StartGame()
{
	isGameOn = true;
	Move candidate;
	Square blockerCandidate;
	while (!(bd.isCheckMate() || bd.isDraw())) {
		blockerCandidate = (bd.isWhiteToMove ? black : white).ThinkBlocker(bd);
		try {
			bd.MoveBlocker(blockerCandidate);
		}
		catch (std::invalid_argument invalidMove) {

			if (invalidMove.what() == "Illegal blocker move") {
				rs.winner = (bd.isWhiteToMove ? Winner::Black : Winner::White);
				rs.reason = Reason::IllegalMove;
				return rs;
			}
			else
				throw invalidMove;
		}
		candidate = (bd.isWhiteToMove ? white : black).Think(bd);
		try {
			bd.MakeMove(candidate);
		}
		catch (std::invalid_argument invalidMove) {
			if (invalidMove.what() == "Illegal move") {
				rs.winner = (bd.isWhiteToMove ? Winner::Black : Winner::White);
				rs.reason = Reason::IllegalMove;
				return rs;
			}
			else
				throw invalidMove;
		}
	}
	isGameOn = false;
	if (bd.isCheckMate()) {
		rs.reason = Reason::Checkmate;
		rs.winner = bd.isWhiteToMove ? Winner::Black : Winner::White;
	}
	else if (bd.is50MoveRule()) rs.reason = Reason::FiftyMoveRule;
	else if (bd.isInsufficientMaterial()) rs.reason = Reason::InsufficientMaterial;
	else if (bd.isSteelMate()) rs.reason = Reason::SteelMate;
	return rs;
}
