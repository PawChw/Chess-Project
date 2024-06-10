#pragma once
#include "../../Chess.h"

class IPlayer {
public:
	IPlayer() = delete;
	virtual Move Think(Board bd) = 0;
	virtual ~IPlayer() = default;
};