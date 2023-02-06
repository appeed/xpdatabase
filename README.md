# XPDatabase
Loads X-Plane database and saves it into binary optimized format for XPFlightPlanner.

XPFlightplanner is a flight planner application for generating flight plans compatible with X-Plane simulator. It has been carefully designed to be intuitive, allowing flightsim enthusiasts to easily create flight plans from one airport to another with just a few simple clicks.

Most features are available for free!

Download at: https://www.xpflightplanner.com

# Install

This software has been tested using Qt 6.4.2. To compile, simply use the standard qmake process.

```
qmake
make
```

# Usage

```
Usage: ./xpdatabase [options] xppath version binpath
Loads X-Plane database and saves it into binary optimized format for XPFlightPlanner.

Options:
  -h, --help  Displays help on commandline options.
  --help-all  Displays help including Qt specific options.

Arguments:
  xppath      Path to X-Plane installation directory
  version     Target version of XPFlightPlanner (x.y.z)
  binpath     Path to binary output file
```

# License

This software and the database are released under the GPLv3 license.

Navigation data from X-Plane are released under the GPL license: https://developer.x-plane.com/docs/data-development-documentation/
