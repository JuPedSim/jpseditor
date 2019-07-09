[![Build Status](https://gitlab.version.fz-juelich.de/jupedsim/jpseditor/badges/develop/build.svg)](https://gitlab.version.fz-juelich.de/JuPedSim/jpseditor/pipelines)[![GitHub license](https://img.shields.io/badge/license-GPL-blue.svg)](https://raw.githubusercontent.com/JuPedSim/jpscore/master/LICENSE)

# JPSeditor
====================


`JPSeditor` is a graphical user interface to create the geometry of a scenario simulated by JuPedSim. It comes with set of CAD- and further tools to simplify the creation of proper xml-files 
incorporating information about the scenario' geometry.


## Features

- DXF Import / Export
- Drawing and define elements for JPScore
- CAD features
	- Grid mode
	- Object snap
	- Orthomode
	- Zooming 
	- Line editing
	- Import background image
- Tools for automatic room and door declarations
- Inifile GUI generator

## System Requirements

See [Requirements](http://www.jupedsim.org/2016-11-03-requirements.html).


## Installing

### Install from source code

The compilation is fairly straightforward: 

- Create a new directory and change to it
```bash
mkdir build && cd build
```

- Run ```CMake``` to build the project
``` bash
cmake .. 
``` 
*cmake* will check if all required  libraries are installed. 

- Compile the project
If the previous step terminated successfully run 
``` bash
make -j
``` 

### Install with installer

We provide binary installer for macOS and windows platform, check the [website](https://github.com/JuPedSim/jpseditor/releases) and download installer for your platform

## Support

Please open an issue using the [issue-tracker](https://github.com/JuPedSim/jpseditor/issues)

## Tutorials

To highlight some features of JuPedSim we have uploaded some videos on our [YouTube channel](https://www.youtube.com/channel/UCKS8w8CUClHEeN4K1SUSMBA) including a tutorial showing how to use the editor.

You can also find more infomations on our [website](http://www.jupedsim.org/jpseditor/)
