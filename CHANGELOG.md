# Change Log
All notable changes to this project will be documented in this file.

## v0.8.7 [08.03.2019]
### Added
- Add line snapping options #102
- Add backuping with lines #101
- Add layers controlling #90

### Changed
- Change warning for Hline #117
- Show operations message on statusBar #113
- Redesign Inifile widget Layout #112

### Bugs fixed
- Fix keyshorts conflict #120
- Fix segmentation fault #121
- Fix error by inputing coordinates #116

## v0.8.6 [08.01.2019]
### Added
- Add zoom extents option #64
- Add zoom Windows option #64
- Add wirte ini-files feature #27

### Changed
- Improve statbility #105
- Use panning mode by middle button 

### Bugs fixed
- Fix shortcut for "Del"

## v0.8.5 [02.11.2018]
### Added
- Add Auto Saving function #67

- Better Panning Mode #100

### Changed
- Update Shortcuts #92

- Design new origin and axis #99

### Bugs fixed
- Esc doesn't work while drawing #98

- SegFault: Delete all Rooms #91


## v0.8.4 [05.10.2018]
### Added
- In menu bar, under "Edit" you will find "Clear Rooms and Doors button" to clear all defined rooms and doors. #54

- Define points of a line using keyboard on status bar. #61 #63

### Changed
- A new tidily icons are added. #88

### Bugs fixed
- Fixed a crash when deleting rooms. #42

- Fixed a problem when width of lines change after highlighting. #50

- Fixed a problem when room is deleted bug caption is still there. #79

- Fixed a problem when captions are overlapped. #82

## v0.8.3 [16.05.2018]
### Added
- Now user can type x and y to define endpoint for line and Hline.  !7

### Changed
-  Defining length is removed for the feature !7

### Bug fixed
- Add Origin and X Y Axis Marks !6

## v0.8.2 [06.11.2017]

### Added 
- Automatic stair definition: 1. define type (stair) and two different elevations of the rooms.
- Aug 2017 Automatic room definition (still buggy)
- Jul 2017 Auto door and obstacle assignment

### Changed
- Jul 2017 Layout of roomwidget
- Jul 2017 Room will be highlighted when selected in roomwidget
- Jul 2017 Autosave temporarely disabled

### Bugs fixed
- Jul 2017 Heap error when deleting room


## v0.8.1 [11.10.2016]

### Added

### Changed

### Bugs fixed

- Heap error when using undo/redo while drawing

### Bugs

- Several heap errors
- Autosave not working properly
- DXF-Import restricted 
    
## v0.8.0 [05.03.2016]

### Added
- 1.2.16 Undo/Redo when Line was edited
- 1.2.16 Change Info Text
- 1.2.16 Load HLines
- 1.2.16 Create and save HLines
- First approaches of an undo/redo framework
	- Undo/Redo working when line added or deleted
- DXF import with different layers - Not working properly with AutoCAD ?! 
- Bug fixes
- CAD features
	- Point and line grid
	- object snap
	- Orthomode
	- Zooming 
	- Line editing
	- Length of line is displayed
	- Show origin
	- ...
- Room and door declarations