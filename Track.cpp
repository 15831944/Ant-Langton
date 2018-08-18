#include "stdafx.h"
#include "Track.h"

Track::Track()
{
}

Track::~Track()
{
}

Position &Track::Return(const Position &pos) const
{
	Track::const_iterator iter = find(TrackItem(pos, Position::Direct::UNKNOWN));

	if (!(iter == end()))
		return (*iter)._position.Return((*iter)._direct);
	else
		return Position((*begin())._position);
}
