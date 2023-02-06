#ifndef XPLOADER_H
#define XPLOADER_H

#include <QObject>
#include "database.h"

#define NO_ERROR 0
#define ERR_NOFILE_XPAPT  (1<<0)
#define ERR_NOFILE_XPFIX  (1<<1)
#define ERR_NOFILE_XPNAV  (1<<2)
#define ERR_NOFILE_XPAWY  (1<<3)

class XpLoader : public QObject
{
	Q_OBJECT
public:
	XpLoader(Database*);
	void load(const QString, const bool);

private:
	void constructFilesPath(const QString, const bool);
	int hasXpDataFiles();
	Airport* addAirportWhenComplete(Airport*, unsigned*, const QGeoPath&);
	int loadAirportsFromXP();
	int loadFixFromXP();
	int loadNavFromXP();
	int loadAwyFromXP();

private:
	Database* db;

	QString xpAptPath;
	QString xpAptPathV11;
	QString xpAptPathV12;
	QString xpFixPath;
	QString xpNavPath;
	QString xpAwyPath;

	QString fixDataCycle;
	QString awyDataCycle;
};

#endif // XPLOADER_H
