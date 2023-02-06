#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <locale.h>
#include "database.h"
#include "xploader.h"

int main(int argc, char *argv[])
{
	// Set the app name
	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("xpdatabase");

	// Command line arguments
	QCommandLineParser parser;
	parser.setApplicationDescription("Loads X-Plane database and saves it into binary optimized format for XPFlightPlanner.");
	parser.addHelpOption();
	parser.addPositionalArgument("xppath", "Path to X-Plane installation directory");
	parser.addPositionalArgument("version", "Target version of XPFlightPlanner (x.y.z)");
	parser.addPositionalArgument("binpath", "Path to binary output file");
	parser.process(app);

	// Test mandatory arguments
	QStringList args = parser.positionalArguments();
	if(args.count() != 3) parser.showHelp(-1);

	// Avoid decimal part with ',' on some european languages
	setlocale(LC_ALL, "en_US.UTF-8");

	// Get the mandatory arguments
	QString xpPath = args.at(0);
	QString version = args.at(1);
	QString binPath = args.at(2);

	// Declare database and xploader
	Database db(binPath);
	XpLoader xploader(&db);

	// Load data from XP and save to binary
	xploader.load(xpPath, false);
	db.saveToBinary(version);

	// Done
	return 0;
}
