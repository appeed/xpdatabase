#include "point.h"

Point::Point(int type, double latitude, double longitude, QString code, QString terminal, QString region, QString rawname, int elevation, double frequency)
{
	// Store properties
	this->uid = POINT_UID(code, terminal, region);
	this->code = code;
	this->position = QGeoCoordinate(latitude, longitude);
	this->terminal = terminal;
	this->region = region;
	this->rawname = rawname;
	this->type = type;
	this->elevation = elevation;
	this->frequency = frequency;
}

QString Point::getUID() const
{
	return uid;
}

void Point::updatePosition(const QGeoCoordinate& position)
{
	this->position = position;
}

QDataStream& operator<<(QDataStream& out, const Point* v)
{
	out << v->code;
	out << v->position;
	out << v->terminal;
	out << v->region;
	out << v->rawname;
	out << v->type;
	out << v->elevation;
	out << v->frequency;
	return out;
}
