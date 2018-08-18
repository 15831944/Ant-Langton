#include "stdafx.h"
#include "TrackItem.h"

TrackItem::TrackItem()
{
	_direct = Position::Direct::UNKNOWN;
	_position = Position(0, 0);
	_setFail = {};
}

TrackItem::TrackItem(const TrackItem &item)
	: TrackItem(item._position, item._direct, item._setFail)
{
}

TrackItem::TrackItem(const Position &pos, const Position::Direct &direct)
	: TrackItem()
{
	_direct = direct;
	_position = pos;
	_setFail = {};
}

TrackItem::TrackItem(const Position &pos, const Position::Direct &direct, const std::set<Position::Direct> &setFail)
	: TrackItem(pos, direct)
{
	std::set<Position::Direct>::const_iterator iter = setFail.begin();
	while (!(iter == setFail.end())) {
		_setFail.insert(*iter);
		iter++;
	}
}

TrackItem::~TrackItem()
{
}

TrackItem &TrackItem::operator= (const TrackItem &trackItem)
{
	_direct = trackItem._direct;
	_position = Position(trackItem._position);
	_setFail = trackItem._setFail;

	return *this;
}