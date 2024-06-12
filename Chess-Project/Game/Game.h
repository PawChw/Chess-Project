#pragma once
#include <stdexcept>

#include "../Chess.h"
#include "IGame.h"
#include "Player/IPlayer.h"

class Game : public IGame {
public:
	std::shared_ptr<IPlayer> white;
	std::shared_ptr<IPlayer> black;
	Game() = delete;
	Game(std::shared_ptr<IPlayer> white, std::shared_ptr<IPlayer> black);
	GameTerminalState StartGame() override;
	GameTerminalState RestartGame() override;
};