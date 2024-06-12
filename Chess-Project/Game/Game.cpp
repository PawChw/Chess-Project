#include "Game.h"

Game::Game(std::shared_ptr<IPlayer> white, std::shared_ptr<IPlayer> black) : IGame(Board()), white(white),
	black(black)
{
	isGameOn = false;
	stateChanged = true;
}

GameTerminalState Game::StartGame()
{
	isGameOn = true;
	Move candidate;
	while (!(bd.isCheckMate() || bd.isDraw())) {
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
				rs.reason =  Reason::IllegalMove;
				return rs;
			}
			else
				throw invalidMove;
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

GameTerminalState Game::RestartGame()
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
