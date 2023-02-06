#include "runway.h"

Runway::Runway(QString airportuid, QGeoCoordinate coord0, QString code0, QGeoCoordinate coord1, QString code1, double width, int surface)
{
	// Store properties
	this->uid = RUNWAY_UID(airportuid, code0, code1);
	this->airportuid = airportuid;
	this->codes.append(code0);
	this->codes.append(code1);
	this->coords.append(coord0);
	this->coords.append(coord1);
	this->width = width;
	this->surface = surface;

	// New asphalt surfaces
	if(surface>=20 && surface<=38) this->surface = ASPHALT;
}

QString Runway::getUID() const
{
	return uid;
}

QDataStream& operator<<(QDataStream& out, const Runway* v)
{
	out << v->airportuid;
	out << v->coords.at(0);
	out << v->coords.at(1);
	out << v->codes.at(0);
	out << v->codes.at(1);
	out << v->width;
	out << v->surface;
	return out;
}
