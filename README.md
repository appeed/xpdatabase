# XPDatabase
Loads X-Plane database and saves it into binary optimized format for XPFlightPlanner.

## Install

This software is tested with Qt6.4.2.

To compile, use the standard qmake way.

```
qmake
make
```

## Usage

```
Usage: ./xpdatabase [options] xppath binpath
Loads X-Plane database and saves it into binary optimized format for XPFlightPlanner.

Options:
  -h, --help  Displays help on commandline options.
  --help-all  Displays help including Qt specific options.
  -p <file>   Dump all the points to a text file
  -s <file>   Dump all the segments to a text file

Arguments:
  xppath      Path to X-Plane installation directory
  binpath     Path to binary output file
```

## License

This software and related data are released under the GPLv3 license.
