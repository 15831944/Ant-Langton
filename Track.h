#pragma once
#include "Position.h"
class Track :
	public Position, std::deque<std::tuple<Position::Direct, std::set<Position::Direct>>>
{
public:
	Track();

	virtual ~Track();
};

