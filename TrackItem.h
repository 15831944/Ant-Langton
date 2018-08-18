#pragma once

#include <set>
#include "Position.h"

struct TrackItem
{
public:

	TrackItem();

	TrackItem(const TrackItem &);

	TrackItem(const Position & , const Position::Direct & );

	TrackItem(const Position & , const Position::Direct & , const std::set<Position::Direct> & );

	virtual ~TrackItem();
	
	Position::Direct _direct;
	
	Position _position;
	
	mutable std::set<Position::Direct> _setFail;
	/* Установить признак нневозможности пердвигаиться из точки по направлению
	* - direct - направление
	*/
	inline void Fail(const Position::Direct &direct) { _setFail.insert(direct); }
	/* Возвратить признак отсутствия возможности передвигаться из точки по любому направлению
	* - sz - кол-во возможных/доступных направлений движения
	*/
	inline bool TrackItem::IsFail(const int&sz)	{	return _setFail.size() == sz;	}

	/* Возвратить признак отсутствия возможности передвигаться из точки (не выполняются условия ограничений) по направлению
	* - direct - направление
	*/
	inline bool TrackItem::IsFail(const Position::Direct &direct)	{	return !(_setFail.find(direct) == _setFail.end());	}

	TrackItem & operator= (const TrackItem &);

	inline bool operator< (const TrackItem &item) const { return _position < item._position; }
};

