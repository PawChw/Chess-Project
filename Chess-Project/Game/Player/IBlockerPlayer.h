#pragma once

#include "IPlayer.h"

class IBlockerPlayer : public IPlayer {
public:
	virtual Square ThinkBlocker(Board bd) = 0;
};