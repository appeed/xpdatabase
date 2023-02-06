#include "boundary.h"

Boundary::Boundary()
{
}

QString Boundary::getUID() const
{
	return "";
}

void Boundary::addCoordinate(const QGeoCoordinate& coord)
{
	path.addCoordinate(coord);
}

QDataStream& operator<<(QDataStream& out, const Boundary* v)
{
	out << v->path;
	return out;
}
