#ifndef TAXIWAY_H
#define TAXIWAY_H

#include <QDataStream>
#include <QGeoCoordinate>
#include <QGeoPath>
#include <QObject>

class Taxiway : public QObject
{
	Q_OBJECT
public:
	Taxiway(int);
	QString getUID() const;
	
	void addCoordinate(QGeoCoordinate);

	friend QDataStream& operator<<(QDataStream&, const Taxiway*);

private:
	QGeoPath path;
	int surface;
};

#endif // TAXIWAY_H
