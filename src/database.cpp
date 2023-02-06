#include <QCoreApplication>
#include <QFile>
#include "database.h"

Database::Database(const QString path)
{
	this->path = path;
}

void Database::setCycle(const QString cycle)
{
	this->cycle  = cycle;
}

QString Database::getCycle() const
{
	return cycle;
}

void Database::addBoundary(Boundary* b)
{
	boundaryModel.add(b->getUID(), b);
}

void Database::addPoint(Point* p)
{
	pointModel.add(p->getUID(), p);
}

void Database::addRunway(Runway* r)
{
	runwayModel.add(r->getUID(), r);
}

void Database::addSegment(Segment* s)
{
	segmentModel.add(s->getUID(), s);
}

void Database::addTaxiway(Taxiway* t)
{
	taxiwayModel.add(t->getUID(), t);
}

Point* Database::getPointFromUID(const QString uid)
{
	Point* p = pointModel.value(uid);
	if(!p) qWarning() << "Point with UID" << uid << "not found in database!";
	return p;
}

Segment* Database::getSegmentFromUID(const QString uid)
{
	Segment* s = segmentModel.value(uid);
	if(!s) qWarning() << "Segment with UID" << uid << "not found in database!";
	return s;
}

bool Database::saveToBinary(const QString version) const
{
	QFile bin(path);
	if(!bin.open(QIODevice::WriteOnly)) return false;
	QDataStream out(&bin);
	out << (int)QT_VERSION_MAJOR;
	out << version;
	out << cycle;
	out << pointModel;
	qInfo() << "Saved" << pointModel.count() << "points";
	out << segmentModel;
	qInfo() << "Saved" << segmentModel.count() << "segments";
	out << runwayModel;
	qInfo() << "Saved" << runwayModel.count() << "runways";
	out << boundaryModel;
	qInfo() << "Saved" << boundaryModel.count() << "boundaries";
	out << taxiwayModel;
	qInfo() << "Saved" << taxiwayModel.count() << "taxiways";
	bin.close();
	qInfo() << "Database saved to" << path;
	return true;
}
