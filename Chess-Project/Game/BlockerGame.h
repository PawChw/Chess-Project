#pragma once
#include "IGame.h"
#include "../Chess.h"
#include "Player/IBlockerPlayer.h"

class BlockerGame : public IGame {
public:
	std::shared_ptr<IBlockerPlayer> white;
	std::shared_ptr<IBlockerPlayer> black;
	BlockerGame() = delete;
	BlockerGame(std::shared_ptr<IBlockerPlayer> white, std::shared_ptr<IBlockerPlayer> black);
	GameTerminalState StartGame() override;
	GameTerminalState RestartGame() override;
};