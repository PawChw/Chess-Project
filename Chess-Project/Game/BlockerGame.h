#pragma once
#include "IGame.h"
#include "../Chess.h"
#include "Player/IBlockerPlayer.h"

class BlockerGame : public IGame {
public:
	Board bd;
	IBlockerPlayer& white;
	IBlockerPlayer& black;
	BlockerGame() = delete;
	BlockerGame(IBlockerPlayer& white, IBlockerPlayer& black);
	GameTerminalState StartGame() override;
};