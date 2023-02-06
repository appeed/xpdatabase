#ifndef POINT_H
#define POINT_H

#include <QDataStream>
#include <QGeoCoordinate>
#include <QObject>

#define NO_TERMINAL QString("ENRT")
#define NO_REGION QString("*")
#define POINT_UID(code, terminal, region) ((region)==NO_REGION?(code):((terminal)==NO_TERMINAL?(code)+"_"+(region):(code)+"_"+(terminal)+"_"+(region)))


class Point : public QObject
{
	Q_OBJECT
public:
	enum Types
	{
		UNKNOWN = 0,
		AIRP = 1,
		NDB = 2,
		VOR = 3,
		ILS = 4,
		LOC = 5,
		GS = 6,
		OM = 7,
		MM = 8,
		IM = 9,
		FIX = 11,
		DMEILS = 12,
		DME = 13,
		COORD = 28
	};

public:
	Point(int, double, double, QString, QString, QString, QString, int = 0, double = 0);
	QString getUID() const;

	void updatePosition(const QGeoCoordinate&);

	friend QDataStream& operator<<(QDataStream&, const Point*);

private:
	QString uid;
	QString code;
	QString rawname;
	QGeoCoordinate position;
	QString terminal;
	QString region;
	int type;
	int elevation;
	double frequency;
};

#endif // POINT_H
