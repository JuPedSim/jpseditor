[![Build Status](https://gitlab.version.fz-juelich.de/jupedsim/jpseditor/badges/develop/build.svg)](https://gitlab.version.fz-juelich.de/JuPedSim/jpseditor/pipelines)[![GitHub license](https://img.shields.io/badge/license-GPL-blue.svg)](https://raw.githubusercontent.com/JuPedSim/jpscore/master/LICENSE)

JPSeditor
====================


`JPSeditor` is a graphical user interface to create the geometry of a scenario simulated by JuPedSim. It comes with set of CAD- and further tools to simplify the creation of proper xml-files 
incorporating information about the scenario' geometry.


Features
========

- DXF Import / Export
- CAD features
	- Point and line grid
	- object snap
	- Orthomode
	- Zooming 
	- Line editing
	- ...
- Tools for automatic room and door declarations



System Requirements
==============

See [Requirements](http://www.jupedsim.org/2016-11-03-requirements.html).


Installing
==========

This version comes with an installer for Windows. So just download it, start it and follow the instructions. The installer is only available for windows at the moment. For other architectures (Linux, OSX) you will need to compile the sources.

The compilation is fairly straightforward: 

- Create a new directory and change to it
```javascript
mkdir build && cd build
```

- Run ```CMake``` to build the project
``` javascript
cmake .. 
``` 
*cmake* will check if all required  libraries are installed. 

- Compile the project
If the previous step terminated successfully run 
``` javascript
make -j2
``` 


Tutorials
========

To highlight some features of JuPedSim we have uploaded some videos on our [YouTube channel](https://www.youtube.com/channel/UCKS8w8CUClHEeN4K1SUSMBA) including a tutorial showing how to use the editor.
