#include "taxiway.h"

Taxiway::Taxiway(int surface)
{
	this->surface = surface;
}

QString Taxiway::getUID() const
{
	return "";
}

void Taxiway::addCoordinate(QGeoCoordinate coord)
{
	path.addCoordinate(coord);
}

QDataStream& operator<<(QDataStream& out, const Taxiway* v)
{
	out << v->path;
	out << v->surface;
	return out;
}
