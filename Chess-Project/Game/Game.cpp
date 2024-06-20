#include "Game.h"

Game::Game(const Game& other)
{
	bd = other.bd;
	is_game_on = other.is_game_on.load();
	player1_is_white = other.player1_is_white;
	player1_score = other.player1_score;
	player2_score = other.player2_score;
	draw_score = other.draw_score;
	rs = other.rs;
	capture = other.capture;
	castle = other.castle;
	check = other.check;
	move = other.move;
	audio_player = other.audio_player;
	white = other.white;
	black = other.black;
}

Game::Game(IPlayer* white, IPlayer* black) : IGame(Board()), white(white),
	black(black)
{
	is_game_on = false;
	state_changed = true;
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
	is_game_on = true;
	Move candidate;
	while (!(bd.IsCheckMate() || bd.IsDraw()) && is_game_on) {
		while (state_changed) {
			std::this_thread::sleep_for(std::chrono::milliseconds(33));
		}
		candidate = (bd.is_white_to_move ? white : black)->Think(bd);
		if (!is_game_on) break;
		try {
			bd.MakeMove(candidate);
			if(candidate.captured_piece)
				audio_player.setBuffer(capture);
			else if(candidate.is_castle.toBool())
				audio_player.setBuffer(castle);
			else if (bd.IsInCheck(bd.is_white_to_move))
				audio_player.setBuffer(check);
			else
				audio_player.setBuffer(move);
			audio_player.play();
			state_changed = true;
		}
		catch (std::invalid_argument invalidMove) {
			if (bd.is_white_to_move) {
				if (!player1_is_white) {
					player1_score++;
				}
				else {
					player2_score++;
				}
				rs.winner = Winner::Black;
			}
			else {
				if (player1_is_white) {
					player1_score++;
				}
				else {
					player2_score++;
				}
				rs.winner = Winner::White;
			}
			rs.reason =  Reason::IllegalMove;
			return rs;
		}
	}
	is_game_on = false;
	if (bd.IsCheckMate()) {
		rs.reason = Reason::checkmate;
		if (bd.is_white_to_move) {
			if (!player1_is_white) {
				player1_score++;
			}
			else {
				player2_score++;
			}
			rs.winner = Winner::Black;
		}
		else {
			if (player1_is_white) {
				player1_score++;
			}
			else {
				player2_score++;
			}
			rs.winner = Winner::White;
		}
	}
	else if (bd.Is50MoveRule()) {
		rs.reason = Reason::InsufficientMaterial;
		draw_score++;
	}
	else if (bd.IsInsufficientMaterial()) {
		rs.reason = Reason::InsufficientMaterial;
		draw_score++;
	}
	else if (bd.IsSteelMate()) {
		rs.reason = Reason::SteelMate;
		draw_score++;
	}
	else {
		draw_score++;
	}
	state_changed = true;
	return rs;
}

GameTerminalState Game::RestartGame()
{
	std::swap(white, black);
	player1_is_white = !player1_is_white;
	bd = Board();
	rs = GameTerminalState();
	state_changed = true;
	return StartGame();
}

std::unique_ptr<IGame> Game::Clone() const
{
	return std::make_unique<Game>(*this);
}
