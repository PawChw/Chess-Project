#include "BlockerGame.h"

BlockerGame::BlockerGame(IBlockerPlayer& white, IBlockerPlayer& black) : bd(Board()), white(white), black(black)
{
}

GameTerminalState BlockerGame::StartGame()
{
	Move candidate;
	Square blockerCandidate;
	while (!(bd.isCheckMate() || bd.isDraw())) {
		blockerCandidate = (bd.isWhiteToMove ? black : white).ThinkBlocker(bd);
		try {
			bd.MoveBlocker(blockerCandidate);
		}
		catch (std::invalid_argument invalidMove) {

			if (invalidMove.what() == "Illegal blocker move")
				return { bd.isWhiteToMove ? Winner::White : Winner::Black, Reason::IllegalMove };
			else
				throw invalidMove;
		}
		candidate = (bd.isWhiteToMove ? white : black).Think(bd);
		try {
			bd.MakeMove(candidate);
		}
		catch (std::invalid_argument invalidMove) {
			if (invalidMove.what() == "Illegal move")
				return { bd.isWhiteToMove ? Winner::Black : Winner::White, Reason::IllegalMove };
			else
				throw invalidMove;
		}
	}
	Reason rs = Reason::Repetition;
	if (bd.isCheckMate()) rs = Reason::Checkmate;
	else if (bd.is50MoveRule()) rs = Reason::FiftyMoveRule;
	else if (bd.isInsufficientMaterial()) rs = Reason::InsufficientMaterial;
	else if (bd.isSteelMate()) rs = Reason::SteelMate;
	return { bd.isWhiteToMove ? Winner::Black : Winner::White, rs };
}
