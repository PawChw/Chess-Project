#pragma once
#include <stdexcept>

#include "../Chess.h"
#include "IGame.h"
#include "Player/IPlayer.h"

class Game final : public IGame {
public:
	IPlayer* white;
	IPlayer* black;
	Game() = delete;
	Game(const Game& other);
	Game(IPlayer* white, IPlayer* black);
	GameTerminalState StartGame() override;
	GameTerminalState RestartGame() override;
	std::unique_ptr<IGame> Clone() const override;
};