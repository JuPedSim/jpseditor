---
layout: post
title: Importing error from JPSvis to JPSeditor
permalink: 2018-10-02-Known_Issue_1.html
published: true
date: '2018-10-02'
---

If you are the user of JPSeditor and JPSvis at the same, maybe you will encounter a terrible situation: The geometry file can read in [JPSvis](http://www.jupedsim.org/jpsvis/), but can't be in JPSeditor.

The reason of this situation is that parser class for geometry file in JPSvis and JPSeditor are NOT same and work in different logic. There is no problem when geometry from JPSeditor is imported, but if the geometry file is wrote in text editor, there could be a problem, even this geometry can be used in [JPScore](http://www.jupedsim.org/jpscore/) and [JPSvis](http://www.jupedsim.org/jpsvis/).

A useful solution for now is making sure that you use the correct grammar for JPSeditor to draw a polygon, whatever it's a line of rooms and obstacles. It means, every polygon should and only can have **two vertexes**, like this:

```xml
<polygon caption="wall1">
    <vertex px="-20.00" py="-2.00"/>
    <vertex px="-20.00" py=" 7.00"/>
</polygon>
```

Here a false (doesn't work in JPSeditor) example:

```xml
<polygon>
    <vertex px="-9.0" py="-1.0"/>
    <vertex px="-9.0" py=" 1.5"/>
    <vertex px="-8.0" py=" 1.5"/>
    <vertex px="-8.0" py=" 0.0"/>
    <vertex px=" 9.0" py=" 0.0"/>
    <vertex px=" 9.0" py="-1.0"/>
    <vertex px="-9.0" py="-1.0"/>
</polygon>
``` 

Sorry for this problem and if you have more questions, please contact <t.zhong@fz-juelich.de>.