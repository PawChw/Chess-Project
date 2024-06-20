#pragma once
#include "IGame.h"
#include "../Chess.h"
#include "Player/IBlockerPlayer.h"

class BlockerGame final : public IGame {
public:
	IBlockerPlayer* white;
	IBlockerPlayer* black;
	BlockerGame() = delete;
	BlockerGame(const BlockerGame& other);
	BlockerGame(IBlockerPlayer* white, IBlockerPlayer* black);
	GameTerminalState StartGame() override;
	GameTerminalState RestartGame() override;
	std::unique_ptr<IGame> Clone() const override;
};