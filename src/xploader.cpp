#include <QDebug>
#include <QFile>
#include <QGeoCoordinate>
#include <QGeoPath>
#include "xploader.h"

XpLoader::XpLoader(Database* db)
{
	this->db = db;
}

void XpLoader::load(const QString path, const bool custom)
{
	// Construct file path
	constructFilesPath(path, custom);

	// Check if files exists
	int status = hasXpDataFiles();

	// Load data
	if(status == NO_ERROR)
	{
		status |= loadAirportsFromXP();
		status |= loadFixFromXP();
		status |= loadNavFromXP();
		status |= loadAwyFromXP();
	}

	// Check for errors
	if(fixDataCycle != db->getCycle() || awyDataCycle != db->getCycle())
	{
		qWarning() << "WARNING: incoherent data cycles among FIX/NAV/AWY files!";
	}
	if((status & ERR_NOFILE_XPAPT) || (status & ERR_NOFILE_XPFIX) || (status & ERR_NOFILE_XPAWY))
	{
		qWarning() << "ERROR: no valid X-Plane installation at given path!";
	}

	// Done
	qWarning() << "Loading done";
}

void XpLoader::constructFilesPath(const QString xpPath, const bool custom)
{
	// Construct sub-path
	QString subPath;
	if(custom) subPath = "/Custom Data";
	else subPath = "/Resources/default data";

	// Construct all file path
	xpAptPathV11 = xpPath + "/Resources/default scenery/default apt dat/Earth nav data" + "/apt.dat";
	xpAptPathV12 = xpPath + "/Global Scenery/Global Airports/Earth nav data" + "/apt.dat";
	xpFixPath = xpPath + subPath + "/earth_fix.dat";
	xpNavPath = xpPath + subPath + "/earth_nav.dat";
	xpAwyPath = xpPath + subPath + "/earth_awy.dat";
}

int XpLoader::hasXpDataFiles()
{
	// Check wether we load V11 or V12 airports (defaut = V12)
	if(QFile::exists(xpAptPathV11)) xpAptPath = xpAptPathV11;
	if(QFile::exists(xpAptPathV12)) xpAptPath = xpAptPathV12;

	// Check files
	if(!QFile::exists(xpAptPath)) return ERR_NOFILE_XPAPT;
	if(!QFile::exists(xpFixPath)) return ERR_NOFILE_XPFIX;
	if(!QFile::exists(xpNavPath)) return ERR_NOFILE_XPNAV;
	if(!QFile::exists(xpAwyPath)) return ERR_NOFILE_XPAWY;
	return NO_ERROR;
}

Airport* XpLoader::addAirportWhenComplete(Airport* a, unsigned* added, const QGeoPath& polygon)
{
	if(!a || !added) return NULL;
	a->updatePosition(polygon.center());
	db->addPoint(a);
	(*added)++;
	return NULL;
}

#define CODE_1(str, char0) (str.at(0)==char0 && (str.size()==1 || (str.size()>1 && str.at(1)==' ')))
#define CODE_3(str, char0, char1, char2) (str.at(0)==char0 && str.at(1)==char1 && str.at(2)==char2 && (str.size()==3 || (str.size()>3 && str.at(3)==' ')))

int XpLoader::loadAirportsFromXP()
{
	// Open the file
	QFile file(xpAptPath);
	qInfo() << "Loading" << file.fileName() << "...";
	if(!file.open(QIODevice::ReadOnly)) return ERR_NOFILE_XPAPT;
	QTextStream in(&file);

	// Read the file header
	if(in.readLine() != "I") qWarning() << "APT: probable wrong file format!";

	// Get the version
	QString version = in.readLine();
	if(!version.startsWith("1100") && !version.startsWith("1200"))
	{
		qWarning() << "APT: probable wrong file format version!" << version;
	}

	// Load all the airports
	Airport* a = NULL;
	Taxiway* t = NULL;
	Boundary* b = NULL;
	typedef enum {NONE, BOUNDARY, TAXIWAY} polygon_state_t;
	polygon_state_t polygon_state = NONE;
	QGeoPath polygon;
	unsigned added_a = 0;
	unsigned added_r = 0;
	while(!in.atEnd())
	{
		// Read the line
		QString line = in.readLine();

		// Ignore empty lines
		if(line.size()<1) continue;

		// Ignore 2XX
		if(line.at(0)=='2') continue;

		// Airport
		if(CODE_1(line, '1'))
		{
			a = addAirportWhenComplete(a, &added_a, polygon);
			QStringList items = line.split(" ", Qt::SkipEmptyParts);
			if(items.count()<6) continue;
			int elevation = items.at(1).toInt();
			QString code = items.at(4);
			items.remove(0, 5);
			QString name = items.join(" ");
			a = new Airport(code, name, elevation);
			polygon.clearPath();
			continue;
		}

		// Ignore lines with less than 3 chars
		if(line.size()<3) continue;

		// Runway
		if(CODE_3(line, '1', '0', '0'))
		{
			if(!a) qWarning() << "APT: potential orphaned runway!";
			QStringList items = line.split(" ", Qt::SkipEmptyParts);
			if(!a || items.count()<20) continue;
			double width = items.at(1).toDouble();
			int surface = items.at(2).toInt();
			QString code0 = items.at(8);
			QGeoCoordinate coord0(items.at(9).toDouble(), items.at(10).toDouble());
			QString code1 = items.at(17);
			QGeoCoordinate coord1(items.at(18).toDouble(), items.at(19).toDouble());
			Runway* r = new Runway(a->getUID(), coord0, code0, coord1, code1, width, surface);
			db->addRunway(r);
			added_r++;
			polygon.addCoordinate(coord0);
			polygon.addCoordinate(coord1);
			continue;
		}

		// Start polygon for the pavement (taxiway)
		if(CODE_3(line, '1', '1', '0'))
		{
			if(!a) qWarning() << "APT: potential orphaned taxiway!";
			if(t) qWarning() << "APT: new taxiway, but previous taxiway was not committed!";
			QStringList items = line.split(" ", Qt::SkipEmptyParts);
			if(items.count()<4) continue;
			int surface = items.at(1).toInt();
			t = new Taxiway(surface);
			polygon_state = TAXIWAY;
			continue;
		}

		// Start polygon for the boundary
		if(CODE_3(line, '1', '3', '0'))
		{
			if(!a) qWarning() << "APT: potential orphaned airport boundary!";
			if(b) qWarning() << "APT: new boundary, but previous boundary was not committed!";
			b = new Boundary();
			polygon_state = BOUNDARY;
			continue;
		}

		// Continues a polygon (111 to 116)
		if(polygon_state!=NONE &&
				(line.at(0)=='1') &&
				(line.at(1)=='1') &&
				(line.at(2)=='1' || line.at(2)=='2' || line.at(2)=='3' || line.at(2)=='4' || line.at(2)=='5' || line.at(2)=='6') &&
				(line.at(3)==' '))
		{
			QStringList items = line.split(" ", Qt::SkipEmptyParts);
			if(items.count()<3) continue;
			double lat = items.at(1).toDouble();
			double lon = items.at(2).toDouble();
			if(line.startsWith("115 ") || line.startsWith("116 ")) qWarning() << "APT: unmanaged line close feature 115 or 116!";
			switch(polygon_state)
			{
				case BOUNDARY:
					if(!b) qWarning() << "APT: new boundary line but no previous boundary header was detected!";
					b->addCoordinate(QGeoCoordinate(lat, lon));
					if(line.startsWith("113 ") || line.startsWith("114 "))
					{
						db->addBoundary(b);
						b = NULL;
						polygon_state = NONE;
					}
					break;
				case TAXIWAY:
					if(!t) qWarning() << "APT: new taxiway line but no previous taxiway header was detected!";
					t->addCoordinate(QGeoCoordinate(lat, lon));
					if(line.startsWith("113 ") || line.startsWith("114 "))
					{
						db->addTaxiway(t);
						t = NULL;
						polygon_state = NONE;
					}
					break;
				default:
					break;
			}
		}
	}
	file.close();

	// Add last airport
	addAirportWhenComplete(a, &added_a, polygon);

	// Debug
	qInfo() << "Loaded" << added_a << "airports with" << added_r << "runways";

	// Success
	return NO_ERROR;
}

int XpLoader::loadFixFromXP()
{
	// Open the file
	QFile file(xpFixPath);
	qInfo() << "Loading" << file.fileName() << "...";
	if(!file.open(QIODevice::ReadOnly)) return ERR_NOFILE_XPFIX;
	QTextStream in(&file);

	// Read the file header
	if(in.readLine() != "I") qWarning() << "FIX: probable wrong file format!";

	// Get the version
	QString version = in.readLine();
	if(!version.startsWith("1100") && !version.startsWith("1101") && !version.startsWith("1200"))
	{
		qWarning() << "FIX: probable wrong file format version!" << version;
	}

	// Get the datacycle
	QStringList items = version.split(" ");
	if(items.count() > 5)
	{
		fixDataCycle = items.at(5);
		fixDataCycle.remove(',');
	}
	else
	{
		qWarning() << "FIX: no data cycle available!" << version;
	}

	// Load all the points
	unsigned added = 0;
	while(!in.atEnd())
	{
		QStringList items = in.readLine().split(" ", Qt::SkipEmptyParts);
		if(items.isEmpty()) continue;
		if(items.count()==1 && items.at(0)=="99") continue;
		if(items.count()<6)
		{
			qWarning() << "FIX: ignored" << items;
			continue;
		}
		double lat = items.at(0).toDouble();
		double lon = items.at(1).toDouble();
		QString code = items.at(2);
		QString terminal = items.at(3);
		QString region = items.at(4);
		QString arinc = items.at(5);
		Q_UNUSED(arinc);
		QString name;
		if(items.count()>=7) name = items.at(6);
		if(code.isEmpty()) continue;
		Point* p = new Point(Point::Types::FIX, lat, lon, code, terminal, region, name);
		db->addPoint(p);
		added++;
	}
	file.close();

	// Debug
	qInfo() << "Loaded" << added << "FIX points, cycle" << fixDataCycle;

	// Success
	return NO_ERROR;
}

int XpLoader::loadNavFromXP()
{
	// Open the file
	QFile file(xpNavPath);
	qInfo() << "Loading" << file.fileName() << "...";
	if(!file.open(QIODevice::ReadOnly)) return ERR_NOFILE_XPNAV;
	QTextStream in(&file);

	// Read the file header
	if(in.readLine() != "I") qWarning() << "NAV: probable wrong file format!";

	// Get the version
	QString version = in.readLine();
	if(!version.startsWith("1100") && !version.startsWith("1101") && !version.startsWith("1150") && !version.startsWith("1200"))
	{
		qWarning() << "NAV: probable wrong file format version!" << version;
	}

	// Get the datacycle
	QStringList items = version.split(" ");
	if(items.count() > 5)
	{
		QString navDataCycle = items.at(5);
		navDataCycle.remove(',');
		db->setCycle(navDataCycle);
	}
	else
	{
		qWarning() << "NAV: no data cycle available!" << version;
	}

	// Load all the points
	unsigned added = 0;
	while(!in.atEnd())
	{
		QStringList items = in.readLine().split(" ", Qt::SkipEmptyParts);
		if(items.isEmpty()) continue;
		if(items.count()==1 && items.at(0)=="99") continue;
		if(items.count()<11)
		{
			qWarning() << "NAV: ignored" << items;
			continue;
		}
		int type = items.at(0).toInt();
		double lat = items.at(1).toDouble();
		double lon = items.at(2).toDouble();
		int elevation = items.at(3).toInt();
		int frequency = items.at(4).toInt();
		QString code = items.at(7);
		QString terminal = items.at(8);
		QString region = items.at(9);
		QString name = items.at(10);
		if(code.isEmpty()) continue;
		if((type == Point::Types::NDB || type == Point::Types::VOR || type == Point::Types::DME))
		{
			Point* p = new Point(type, lat, lon, code, terminal, region, name, elevation, type==Point::Types::NDB ? frequency : ((double)frequency/100.0));
			db->addPoint(p);
			added++;
		}
		//TODO: add ILS to runways
	}
	file.close();

	// Debug
	qInfo() << "Loaded" << added << "NAV points, cycle" << db->getCycle();

	// Success
	return NO_ERROR;
}

int XpLoader::loadAwyFromXP()
{
	// Open the file
	QFile file(xpAwyPath);
	qInfo() << "Loading" << file.fileName() << "...";
	if(!file.open(QIODevice::ReadOnly)) return ERR_NOFILE_XPAWY;
	QTextStream in(&file);

	// Read the file header
	if(in.readLine() != "I") qWarning() << "AWY: probable wrong file format!";

	// Get the version
	QString version = in.readLine();
	if(!version.startsWith("1100") && !version.startsWith("1101"))
	{
		qWarning() << "AWY: probable wrong file format version!" << version;
	}

	// Get the datacycle
	QStringList items = version.split(" ");
	if(items.count() > 5)
	{
		awyDataCycle = items.at(5);
		awyDataCycle.remove(',');
	}
	else
	{
		qWarning() << "AWY: no data cycle available!" << version;
	}

	// Load all the points
	unsigned added = 0;
	while(!in.atEnd())
	{
		QStringList items = in.readLine().split(" ", Qt::SkipEmptyParts);
		if(items.isEmpty()) continue;
		if(items.count()==1 && items.at(0)=="99") continue;
		if(items.count()<11)
		{
			qWarning() << "AWY: ignored" << items;
			continue;
		}
		QString code0 = items.at(0);
		QString region0 = items.at(1);
		int type0 = items.at(2).toInt();
		Q_UNUSED(type0)
		QString code1 = items.at(3);
		QString region1 = items.at(4);
		int type1 = items.at(5).toInt();
		Q_UNUSED(type1)
		QString dir = items.at(6);
		int direction = dir=="F"?Segment::Directions::FORWARD:(dir=="B"?Segment::Directions::BACKWARD:Segment::Directions::NONE);
		if(direction == Segment::Directions::BACKWARD) qWarning() << "Backward routes are not handled!"; //TODO (create the inverted route?)
		int type = items.at(7).toInt();
		int flmin = items.at(8).toInt();
		int flmax = items.at(9).toInt();
		QString code = items.at(10);
		QString uid0 = POINT_UID(code0, NO_TERMINAL, region0);
		QString uid1 = POINT_UID(code1, NO_TERMINAL, region1);
		Point* p0 = db->getPointFromUID(uid0);
		Point* p1 = db->getPointFromUID(uid1);
		if(!p0 || !p1)
		{
			qWarning() << "AWY:" << code << "p0" << p0 << uid0 << "or p1" << p1 << uid1 << "not found in database!";
			continue;
		}
		Segment* s = new Segment(direction, type, uid0, p0, uid1, p1, code, flmin, flmax);
		db->addSegment(s);
		added++;
	}
	file.close();

	// Debug
	qInfo() << "Loaded" << added << "AWY, cycle" << awyDataCycle;

	// Success
	return NO_ERROR;
}
