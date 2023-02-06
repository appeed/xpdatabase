TARGET = xpdatabase

QT -= gui
QT += positioning

CONFIG += c++11 console
CONFIG -= app_bundle

HEADERS += \
	src/abstractmodel.h \
	src/airport.h \
	src/boundary.h \
	src/database.h \
	src/point.h \
	src/xploader.h \
	src/runway.h \
	src/segment.h \
	src/taxiway.h \

SOURCES += \
	src/airport.cpp \
	src/boundary.cpp \
	src/database.cpp \
	src/point.cpp \
	src/xploader.cpp \
	src/runway.cpp \
	src/segment.cpp \
	src/taxiway.cpp \
	src/xpdatabase.cpp \

DESTDIR = .
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
