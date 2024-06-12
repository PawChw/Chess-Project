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
	while (!(bd.isCheckMate() || bd.isDraw())) {
		while (stateChanged) {
			std::this_thread::sleep_for(std::chrono::milliseconds(33));
		}
		blockerCandidate = (bd.isWhiteToMove ? black : white)->ThinkBlocker(bd);
		try {
			bd.MoveBlocker(blockerCandidate);
			stateChanged = true;
		}
		catch (std::invalid_argument invalidMove) {

			if (invalidMove.what() == "Illegal blocker move") {
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
			else{
				std::cout << invalidMove.what() << std::endl;
				throw invalidMove;
			}
		}
		while (stateChanged) {
			std::this_thread::sleep_for(std::chrono::milliseconds(33));
		}
		candidate = (bd.isWhiteToMove ? white : black)->Think(bd);
		try {
			bd.MakeMove(candidate);
			stateChanged = true;
		}
		catch (std::invalid_argument invalidMove) {
			if (invalidMove.what() == "Illegal move") {
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
			else {
				std::cout << invalidMove.what() << std::endl;
				throw invalidMove;
			}
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
		auto& tmp = white;
		white = black;
		black = tmp;
	}
	player1isWhite = !player1isWhite;
	bd = Board();
	rs = GameTerminalState();
	return StartGame();
}
