Note: Package identifiers are as of Ubuntu 16.04 distribution.  They
may differ for other distributions or distribution versions.

Compiling
=========

Requirements
------------

* Qt5 (libraries and include header files) for core, widgets, GUI

* pthread library (library and header files)

* Xerces-C++ (library and include header files) (e.g. Ubuntu packages
libxerces-c3.1, libxerces-c-dev)

* Xalan (library and include header files) (e.g. Ubuntu packages
libxalan-c111, libxalan-c-dev)

* icu-devtools and libicu-dev libraries; but they should by default
  already be installed at least on more recent versions of Raspbian OS
  (e.g. Jessie) -- N.B.: in order to still be able to compile this
  software even on systems that do not have these sensors, you still
  can install (or compile and install) yourself these libraries; in
  this case, the libraries will just do nothing, but this software
  will otherwise still work

* Do not forget to calibrate the magnetometer (and, optionally, also
  the accelerometers).  Without calibration, the sensors may deliver
  faulty data.  For calibration, see e.g. section _Calibration_ on
  this page:

  https://www.raspberrypi.org/documentation/hardware/sense-hat/

  Or, alternatively, use the magnetometer calibration functionality
  of the more recent _RTIMULib2_ library.

Running
=======

* Qt5 libraries for core, widgets, GUI

* pthread library

* Xerces-C++ library (e.g. Ubuntu package libxerces-c3.1)

* Xalan library (e.g. Ubuntu package libxalan-c111)

Note: You can (as with every X application) run maze without X window
directly from text console with the command

  startx /home/pi/maze -- -nocursor -depth 16

(assuming that you have put the maze binary into your home directory).
