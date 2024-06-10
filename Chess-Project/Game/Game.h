#pragma once
#include <cstdint>
#include <stdexcept>

#include "../Chess.h"
#include "Player/IPlayer.h"

enum class Winner : uint8_t {
	None = 0, White, Black
};

enum class Reason : uint8_t {
	InsufficientMaterial, SteelMate, FiftyMoveRule, Repetition, Checkmate, IllegalMove
};

struct GameTerminalState {
	Winner winner;
	Reason reason;
};

class Game {
private:
	Board bd;
	IPlayer& white;
	IPlayer& black;
public:
	Game() = delete;
	Game(IPlayer& white, IPlayer& black);
	virtual GameTerminalState StartGame();
};