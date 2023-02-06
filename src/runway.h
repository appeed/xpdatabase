#ifndef RUNWAY_H
#define RUNWAY_H

#include <QDataStream>
#include <QGeoCoordinate>
#include <QList>
#include <QObject>

#define RUNWAY_UID(airportuid, code0, code1) ((airportuid)+"_"+(code0)+"_"+(code1))

class Runway : public QObject
{
	Q_OBJECT
public:
	enum Surfaces
	{
		UNKNOWN = 0,
		ASPHALT = 1,
		CONCRETE = 2,
		GRASS = 3,
		DIRT = 4,
		GRAVEL = 5,
		DRYLAKEBED = 12,
		WATER = 13,
		SNOWICE = 14,
		TRANSPARENT = 15
	};

public:
	Runway(QString, QGeoCoordinate, QString, QGeoCoordinate, QString, double, int);
	QString getUID() const;

	friend QDataStream& operator<<(QDataStream&, const Runway*);

private:
	QString uid;
	QString airportuid;
	QStringList codes;
	QList<QGeoCoordinate> coords;
	double width;
	int surface;
};

#endif // RUNWAY_H
