---
layout: post
date: '2018-10-17'
title: How to develop JPSeditor on macOS, windows and Linux
permalink: 2018-10-17-develop_JPSeditor.html
published: true
---

<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.3.1/css/all.css" integrity="sha384-mzrmE5qonljUremFsqc01SB46JvROS7bZs3IO2EmfFsd15uHvIt+Y8vEf7N7fWAU" crossorigin="anonymous">

If you want to ust the newest version of [JPSeditor](http://www.jupedsim.org/jpseditor/), or other reasons, maybe you want to build [JPSeditor](http://www.jupedsim.org/jpseditor/) from [source code](https://github.com/JuPedSim/jpseditor). [JPSeditor](http://www.jupedsim.org/jpseditor/) is based on [Qt](https://www.qt.io/) development framework. This short tutorial will show how to build development environment for [JPSeditor](http://www.jupedsim.org/jpseditor/).

## 1. Step: Download Software Development Kits and supporting library

Before the installation of Qt and Qt Creator, you should make sure Software Development Kits for windows, macOS or Linux is installed.
SDK provides the basic kits for Software development, for example, compilers, debuggers.

* For Windows users, download SDK on [Microsoft developer](https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk) website and install it.
* For macOS users, run the script in terminal.app: `xcode-select --install`, then follow the Pop-up windows to install Xcode Command Line Tool.
* For Linux users, run the script in terminal: `sudo apt-get install build-essential`. Then run the script `sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev`.

## 2. Step: Install Qt

For development you need Qt core and IDE, there are many ways to install Qt and Qt Creator (If you want to use it to develop Qt GUI Projects), but the easiest way is by Qt Online Installer.

1. Download Qt Online Installer (open source version) on [Qt official website](https://www.qt.io/download-qt-installer?hsCtaTracking=9f6a2170-a938-42df-a8e2-a9f0b1d6cdce%7C6cb0de4f-9bb5-4778-ab02-bfb62735f3e5).
2. For macOS and windows users, click `.DMG` or `.exe` to execute installation. For Linux users, go to termial.app, move to the folder where Qt Online Installer is, and run the script 

```
chmod +x qt-opensource-linux-x64-5.7.0.run
./qt-opensource-linux-x64-5.7.0.run
```

Qt Online Insteller will guide you to install all components you need, or don't need right now. For developing JPSeditor, you just need to install:

* The newest Qt version, for now (2018.10.17), it's Qt 5.11.2;
* Qt Creator, it's chosen acquiescently

After the installation, you will find the Qt creator app, and Qt MaintenanceTool, with this you can install, uninstall and update all Qt components.

## 3. Setting for [JPSeditor](http://www.jupedsim.org/jpseditor/)

Before opening your Qt Creator, you can download [JPSeditor](http://www.jupedsim.org/jpseditor/) source code on [Github](https://github.com/JuPedSim/jpseditor) or [Gitlab](https://gitlab.version.fz-juelich.de/jupedsim/jpseditor).

In Qt Creator, you can develop like in other IDE, but before you build [JPSeditor](http://www.jupedsim.org/jpseditor/), make sure your 
Build kits work.

Open options setting of Qt Creator, go to `kit`, your can set lots of kits for different purposes. On Qt [website](http://doc.qt.io/qtcreator/creator-targets.html) you can see how to add a kit.

For developing [JPSeditor](http://www.jupedsim.org/jpseditor/), make sure you are using right compiler in the kit:

* For macOS users, Clang is recommended;
* For Windows users, you can use MSVC or MinGW;
* For Linux users, use g++.

If you have questions about [JPSeditor](http://www.jupedsim.org/jpseditor/), please contact <t.zhong@fz-juelich.de>.