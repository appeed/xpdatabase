#ifndef AIRPORT_H
#define AIRPORT_H

#include <QObject>
#include "point.h"


class Airport : public Point
{
	Q_OBJECT
public:
	Airport(QString, QString, int);
};

#endif // AIRPORT_H
