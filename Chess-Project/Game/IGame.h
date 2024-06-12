#pragma once
#include <atomic>
#include <cstdint>
#include<thread>
#include<chrono>
#include "../Chess.h"

enum class Winner : uint8_t {
	None = 0, White, Black
};

enum class Reason : uint8_t {
	Repetition = 0, InsufficientMaterial, SteelMate, FiftyMoveRule, Checkmate, IllegalMove
};

struct GameTerminalState {
	Winner winner;
	Reason reason;
	GameTerminalState() : winner(Winner::None), reason(Reason::Repetition) {}
};

class IGame {
public:
	Board bd;
	std::atomic_bool isGameOn, stateChanged;
	bool player1isWhite = true;
	int player1 = 0, player2 = 0, draw = 0;
	GameTerminalState rs = GameTerminalState();
	IGame() : bd(Board()) {}
	IGame(Board bd) : bd(bd) {}
	virtual GameTerminalState StartGame() = 0;
	virtual GameTerminalState RestartGame() = 0;
	virtual ~IGame() = default;
};