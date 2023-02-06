#include "airport.h"

Airport::Airport(QString code, QString name, int elevation)
	:Point(Point::AIRP, 0, 0, code, NO_TERMINAL, NO_REGION, name, elevation, 0)
{
}
