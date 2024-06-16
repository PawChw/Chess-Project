#include "BlockerGame.h"

BlockerGame::BlockerGame(std::shared_ptr<IBlockerPlayer> white, std::shared_ptr<IBlockerPlayer> black) : IGame(Board()), white(white),
black(black)
{
	isGameOn = false;
	stateChanged = true;
}

GameTerminalState BlockerGame::StartGame()
{
	isGameOn = true;
	Move candidate;
	Square blockerCandidate;
	while (!(bd.isCheckMate() || bd.isDraw()) && isGameOn) {
		while (stateChanged) {
			std::this_thread::sleep_for(std::chrono::milliseconds(33));
		}
		candidate = (bd.isWhiteToMove ? white : black)->Think(bd);
		if (!isGameOn) break;
		try {
			bd.MakeMove(candidate);
		}
		catch (std::invalid_argument invalidMove) {
			if (bd.isWhiteToMove) {
				if (!player1isWhite) {
					player1++;
				}
				else {
					player2++;
				}
				rs.winner = Winner::Black;
			}
			else {
				if (player1isWhite) {
					player1++;
				}
				else {
					player2++;
				}
				rs.winner = Winner::White;
			}
			rs.reason = Reason::IllegalMove;
			return rs;
		}
		stateChanged = true;
		blockerCandidate = (bd.isWhiteToMove ? black : white)->ThinkBlocker(bd);
		if (!isGameOn) break;
		try {
			bd.MoveBlocker(blockerCandidate);
			stateChanged = true;
		}
		catch (std::invalid_argument invalidMove) {
			if (bd.isWhiteToMove) {
				if (!player1isWhite) {
					player1++;
				}
				else {
					player2++;
				}
				rs.winner = Winner::Black;
			}
			else {
				if (player1isWhite) {
					player1++;
				}
				else {
					player2++;
				}
				rs.winner = Winner::White;
			}
			rs.reason = Reason::IllegalMove;
			return rs;
		}
	}
	isGameOn = false;
	if (bd.isCheckMate()) {
		rs.reason = Reason::Checkmate;
		if (bd.isWhiteToMove) {
			if (!player1isWhite) {
				player1++;
			}
			else {
				player2++;
			}
			rs.winner = Winner::Black;
		}
		else {
			if (player1isWhite) {
				player1++;
			}
			else {
				player2++;
			}
			rs.winner = Winner::White;
		}
	}
	else if (bd.is50MoveRule()) {
		rs.reason = Reason::InsufficientMaterial;
		draw++;
	}
	else if (bd.isInsufficientMaterial()) {
		rs.reason = Reason::InsufficientMaterial;
		draw++;
	}
	else if (bd.isSteelMate()) {
		rs.reason = Reason::SteelMate;
		draw++;
	}
	else {
		draw++;
	}
	stateChanged = true;
	return rs;
}

GameTerminalState BlockerGame::RestartGame()
{
	{
		std::swap(white, black);
	}
	player1isWhite = !player1isWhite;
	bd = Board();
	rs = GameTerminalState();
	return StartGame();
}
