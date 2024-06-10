#pragma once
#include <stdexcept>

#include "../Chess.h"
#include "IGame.h"
#include "Player/IPlayer.h"

class Game : public IGame {
public:
	Board bd;
	IPlayer& white;
	IPlayer& black;
	Game() = delete;
	Game(IPlayer& white, IPlayer& black);
	GameTerminalState StartGame() override;
};