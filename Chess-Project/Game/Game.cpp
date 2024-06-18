#include "Game.h"

Game::Game(std::shared_ptr<IPlayer> white, std::shared_ptr<IPlayer> black) : IGame(Board()), white(white),
	black(black)
{
	isGameOn = false;
	stateChanged = true;
	if (!capture.loadFromFile("Assets/Sounds/Default/capture.wav"))
		throw std::runtime_error("Capture audio not found");
	if (!castle.loadFromFile("Assets/Sounds/Default/castle.wav"))
		throw std::runtime_error("Castle audio not found");
	if (!check.loadFromFile("Assets/Sounds/Default/check.wav"))
		throw std::runtime_error("Check audio not found");
	if (!move.loadFromFile("Assets/Sounds/Default/move.wav"))
		throw std::runtime_error("Move audio not found");
}

GameTerminalState Game::StartGame()
{
	sf::Music musicPlayer;
	isGameOn = true;
	Move candidate;
	while (!(bd.isCheckMate() || bd.isDraw()) && isGameOn) {
		while (stateChanged) {
			std::this_thread::sleep_for(std::chrono::milliseconds(33));
		}
		candidate = (bd.isWhiteToMove ? white : black)->Think(bd);
		if (!isGameOn) break;
		try {
			bd.MakeMove(candidate);
			if(candidate.capturedPiece)
				audioPlayer.setBuffer(capture);
			else if(candidate.isCastle.toBool())
				audioPlayer.setBuffer(castle);
			else if (bd.isInCheck(bd.isWhiteToMove))
				audioPlayer.setBuffer(check);
			else
				audioPlayer.setBuffer(move);
			audioPlayer.play();
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
			rs.reason =  Reason::IllegalMove;
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

GameTerminalState Game::RestartGame()
{
	std::swap(white, black);
	player1isWhite = !player1isWhite;
	bd = Board();
	rs = GameTerminalState();
	return StartGame();
}
