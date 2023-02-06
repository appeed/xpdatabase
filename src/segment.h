#ifndef SEGMENT_H
#define SEGMENT_H

#include <QDataStream>
#include <QObject>
#include "point.h"

#define AIRWAY_UID(start, code, end) ((start)+":"+(code)+":"+(end))

#define SEGMENT_UNDEF_ALTITUDE (0)
#define SEGMENT_UNDEF_LEVEL (SEGMENT_UNDEF_ALTITUDE/100)

class Segment : public QObject
{
	Q_OBJECT
public:
	enum Types
	{
		LOW = 1,
		HIGH = 2,
		DRCT = 20,
		TRACK = 21
	};
	enum Directions
	{
		BACKWARD = -1,
		NONE = 0,
		FORWARD = 1
	};

public:
	Segment(int, int, QString, Point*, QString, Point*, QString, int, int);
	QString getUID() const;

	friend QDataStream& operator<<(QDataStream&, const Segment*);

private:
	Point* p0;
	Point* p1;
	QString uid;
	QString code;
	int direction;
	int type;
	int flmin;
	int flmax;
};

#endif // SEGMENT_H
