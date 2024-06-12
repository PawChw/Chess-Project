#pragma once
#include "../../Chess.h"

class IPlayer {
public:
	IPlayer() = default;
	virtual Move Think(Board bd) = 0;
	virtual ~IPlayer() = default;
};