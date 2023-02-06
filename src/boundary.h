#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <QDataStream>
#include <QGeoCoordinate>
#include <QGeoPath>
#include <QObject>

class Boundary : public QObject
{
	Q_OBJECT
public:
	Boundary();
	QString getUID() const;

	void addCoordinate(const QGeoCoordinate&);

	friend QDataStream& operator<<(QDataStream&, const Boundary*);

private:
	QGeoPath path;
};

#endif // BOUNDARY_H
