#include "segment.h"

Segment::Segment(int direction, int type, QString uid0, Point* p0, QString uid1, Point* p1, QString code, int flmin, int flmax)
{
	// Check the point points
	if(!p0 || !p1) return;
	this->p0 = p0;
	this->p1 = p1;

	// Store base properties
	this->uid = AIRWAY_UID(uid0, code, uid1);
	this->code = code;
	this->direction = direction;
	this->type = type;
	this->flmin = flmin;
	this->flmax = flmax;
}

QString Segment::getUID() const
{
	return uid;
}

QDataStream& operator<<(QDataStream& out, const Segment* v)
{
	out << v->p0->getUID();
	out << v->p1->getUID();
	out << v->code;
	out << v->direction;
	out << v->type;
	out << v->flmin;
	out << v->flmax;
	return out;
}
