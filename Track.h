#pragma once

#include <set>
#include "TrackItem.h"

struct Track :
	public std::set<TrackItem>
{
public:

	Track();

	virtual ~Track();

	Position &Return(const Position &) const;
};

