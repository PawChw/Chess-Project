#pragma once
#include <atomic>
#include <cstdint>
#include <stdexcept>
#include<thread>
#include<chrono>
#include<SFML/Audio.hpp>
#include "../Chess.h"

enum class Winner : uint8_t {
	None = 0, White, Black
};

enum class Reason : uint8_t {
	Repetition = 0, InsufficientMaterial, SteelMate, FiftyMoveRule, checkmate, IllegalMove, KingCaptured
};

struct GameTerminalState {
	Winner winner;
	Reason reason;
	GameTerminalState() : winner(Winner::None), reason(Reason::Repetition) {}
};

class IGame {
public:
	Board bd;
	std::atomic_bool is_game_on, state_changed;
	bool player1_is_white = true;
	int player1_score = 0, player2_score = 0, draw_score = 0;
	GameTerminalState rs = GameTerminalState();
	IGame() : bd(Board()) {
	}
	IGame(Board bd) : bd(bd) {}
	sf::SoundBuffer capture, castle, check, move;
	sf::Sound audio_player;
	virtual GameTerminalState StartGame() = 0;
	virtual GameTerminalState RestartGame() = 0;
	virtual ~IGame() = default;
};