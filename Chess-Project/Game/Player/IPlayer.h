#pragma once
#include "../../Chess.h"

class IPlayer {
public:
	virtual Move Think(Board bd) = 0;
	virtual ~IPlayer() = default;
};