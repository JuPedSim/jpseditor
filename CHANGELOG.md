# Change Log
All notable changes to this project will be documented in this file.

## v0.8.4 [05.10.2018]
## Added
- In menu bar, under "Edit" you will find "Clear Rooms and Doors button" to clear all defined rooms and doors.

- Define points of a line using keyboard on status bar.

## Changed
- A new tidily icons are added.

## Bugs fixed
- Fixed a crash when deleting rooms.

- Fixed a problem when width of lines change after highlighting.

- Fixed a problem when room is deleted bug caption is still there.

- Fixed a problem when captions are overlapped.

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