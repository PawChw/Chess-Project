#pragma once
#include <cstdint>

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

class IGame {
public:
	virtual GameTerminalState StartGame() = 0;
	virtual ~IGame() = default;
};