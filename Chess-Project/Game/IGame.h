#pragma once
#include <atomic>
#include <cstdint>

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
	std::atomic_bool isGameOn;
	GameTerminalState rs = GameTerminalState();
	virtual GameTerminalState StartGame() = 0;
	virtual ~IGame() = default;
};