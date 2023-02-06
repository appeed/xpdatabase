#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include "abstractmodel.h"
#include "airport.h"
#include "boundary.h"
#include "point.h"
#include "runway.h"
#include "segment.h"
#include "taxiway.h"

class Database : public QObject
{
	Q_OBJECT
public:
	Database(const QString);

	// Accessors to cycle and counts
	void setCycle(const QString);
	QString getCycle() const;

	// DB add functions
	void addBoundary(Boundary*);
	void addPoint(Point*);
	void addRunway(Runway*);
	void addSegment(Segment*);
	void addTaxiway(Taxiway*);

	// DB queries
	Point* getPointFromUID(const QString);
	Segment* getSegmentFromUID(const QString);

	// Save
	bool saveToBinary(const QString) const;

private:
	// Models
	AbstractModel<Segment> segmentModel;
	AbstractModel<Boundary> boundaryModel;
	AbstractModel<Point> pointModel;
	AbstractModel<Runway> runwayModel;
	AbstractModel<Taxiway> taxiwayModel;

	// Properties
	QString path;
	QString cycle;
};

#endif // DATABASE_H
