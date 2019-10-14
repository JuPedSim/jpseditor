# To have CMake pick a Qt installation of your choice that won't be found
# automatically, set the CMAKE_PREFIX_PATH environment variable.
# For example: "export CMAKE_PREFIX_PATH=/usr/local/trolltech/qt4.3.5"
# Specify the path to qmake with  QT_QMAKE_EXECUTABLE
#
# Available flags (-D):
#================
# 1. CMAKE_BUILD_TYPE=Release (default Debug)
# 2. DESIRED_QT_VERSION=5 (default 4)
# example: >> cmake  -DDESIRED_QT_VERSION=5  -DCMAKE_PREFIX_PATH=~/Qt/5.5/clang_64 ..
# Flags for VS: -G "Visual Studio ..." -DCMAKE_PREFIX_PATH=... -DCMAKE_C_FLAGS="-mwindows"
# Qt5_DIR:PATH
cmake_minimum_required(VERSION 3.2)
# IF (POLICY CMP0048) # in CMake 3.0.0+
#   CMAKE_POLICY (SET CMP0048 OLD) # keep PROJECT() from clearing VERSION variables
# ENDIF (POLICY CMP0048)
# IF (POLICY CMP0043) # in CMake 3.0.0+
#   CMAKE_POLICY (SET CMP0043 OLD) # keep PROJECT() from clearing VERSION variables
# ENDIF (POLICY CMP0043)
# IF (POLICY CMP0020)
#   CMAKE_POLICY (SET CMP0020 OLD) # keep old POLICY (2.8.10 and lower) The OLD behavior for this policy is not to link executables to
# ENDIF (POLICY CMP0020)			#qtmain.lib automatically when they link to the QtCore IMPORTEDtarget

# IF (POLICY CMP0025)
#   cmake_policy(SET CMP0025 NEW) #fixes error No known features for CXX compiler (on mac)
# ENDIF (POLICY CMP0025)

project(JPSeditor LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

MESSAGE( STATUS "System ........................................ ${CMAKE_SYSTEM_NAME} (${CMAKE_SYSTEM_VERSION}, ${CMAKE_SYSTEM_PROCESSOR})" )

set(README_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CMAKE_COLOR_MAKEFILE ON)
set(JPSEDITOR_MAJOR_VERSION 0)
set(JPSEDITOR_MINOR_VERSION 8)
set(JPSEDITOR_PATCH_VERSION 9)
set(JPSEDITOR_VERSION
        ${JPSEDITOR_MAJOR_VERSION}.${JPSEDITOR_MINOR_VERSION}.${JPSEDITOR_PATCH_VERSION})

find_package(Git REQUIRED) # no need for this msg. It comes from cmake.findgit()

find_program(GIT_SCM git DOC "Git version control")
mark_as_advanced(GIT_SCM)
find_file(GITDIR NAMES .git PATHS ${CMAKE_SOURCE_DIR} NO_DEFAULT_PATH)
if (GIT_SCM AND GITDIR)
  # the commit's SHA1, and whether the building workspace was dirty or not
  # describe --match=NeVeRmAtCh --always --tags --abbrev=40 --dirty
  execute_process(COMMAND
          "${GIT_EXECUTABLE}" --no-pager describe --tags --always --dirty
          WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
          OUTPUT_VARIABLE GIT_SHA1
          ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

  # branch
  execute_process(
          COMMAND "${GIT_EXECUTABLE}" rev-parse --abbrev-ref HEAD
          WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
          OUTPUT_VARIABLE GIT_BRANCH
          OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  # the date of the commit
  execute_process(COMMAND
          "${GIT_EXECUTABLE}" log -1 --format=%ad --date=local
          WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
          OUTPUT_VARIABLE GIT_DATE
          ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

  add_definitions("-DGIT_COMMIT_HASH=\"${GIT_SHA1}\"")
  add_definitions("-DGIT_COMMIT_DATE=\"${GIT_DATE}\"")
  add_definitions("-DGIT_BRANCH=\"${GIT_BRANCH}\"")
  add_definitions("-DJPSEDITOR_VERSION=\"${JPSEDITOR_VERSION}\"")
else()
  message(STATUS "Not in a git repo")
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Release)
endif()

message( STATUS "Generating  ................................... ${PROJECT_NAME} (${JPSEDITOR_VERSION}) -- ${CMAKE_BUILD_TYPE}")
if(APPLE )
  set (CMAKE_INSTALL_PREFIX "/Applications")
  set (CMAKE_PREFIX_PATH "/Applications/Qt/5.13.0/clang_64")
endif()
message(STATUS "${PROJECT_NAME} will be installed to ${CMAKE_INSTALL_PREFIX}")


# ------------------------- Begin Generic CMake Variable Logging ------------------
# the compiler used for C files
# message( STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER} )

# the compiler used for C++ files
# message( STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER} )
# ------------------------- End of Generic CMake Variable Logging ------------------

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# put in cmake cmake scripts if any
# message(STATUS "CMAKE_MODULE_PATH" ${CMAKE_MODULE_PATH})

#------------------ set important directories --------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")
# Debug messages
message( STATUS "CMAKE_BINARY_DIR .............................. " ${CMAKE_BINARY_DIR} )
message( STATUS "CMAKE_SOURCE_DIR .............................. " ${CMAKE_SOURCE_DIR} )
message( STATUS "CMAKE_CURRENT_SOURCE_DIR ...................... " ${CMAKE_CURRENT_SOURCE_DIR} )
message( STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY ................ " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
message( STATUS "EXECUTABLE_OUTPUT_PATH ........................ " ${EXECUTABLE_OUTPUT_PATH} )

# we need this to be able to include headers produced by uic in our code
# (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
include_directories( ${CMAKE_BINARY_DIR} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

#--------------------
if (NOT MSVC)
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  else()
    message(AUTHOR_WARNING "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Use a different C++ compiler.")
  endif()
  message(STATUS "Checking for C++11 compiler ................... yes")
endif()
#---------------------

set( SRCS
    src/main.cpp

## Controllers
    src/mainWindow.cpp
    src/widgets/roomwidget.cpp
    src/widgets/settingdialog.cpp
    src/widgets/inifilewidget.cpp
    src/widgets/snappingoptions.cpp
    src/widgets/widgetlandmark.cpp
    src/widgets/widgetsettings.cpp
    src/widgets/roomlistwidget.cpp
    src/widgets/propertywidget.cpp
    src/widgets/transitionwidget.cpp
    src/widgets/layerwidget.cpp
    src/widgets/templatewidget.cpp
    src/widgets/runningwidget.cpp
    src/widgets/goalwidget.cpp
    src/widgets/sourcewidget.cpp

    src/GraphicView.cpp
    src/graphicscene.cpp
    src/datamanager.cpp

## Models
    src/models/jpselementlistmodel.cpp
    src/models/jpssourcelistmodel.cpp
    src/models/jpsgoallistmodel.cpp
    src/models/layer.cpp

## JPSelements
    src/jpszone.cpp
    src/jpscrossing.cpp
    src/jpsLineItem.cpp
    src/jpstransition.cpp
    src/jpsobstacle.cpp
    src/jpslandmark.cpp
    src/jpstrack.cpp
    src/jpsconnection.cpp
    src/jpsregion.cpp
    src/jpssource.cpp
    src/jpsgoal.cpp

##  XML
    src/XML/goalreader.cpp
    src/XML/sourcereader.cpp

## MISC
    dxflib/src/dl_writer_ascii.cpp
    dxflib/src/dl_dxf.cpp
    src/UndoFramework/action.cpp
    src/UndoFramework/actionstack.cpp
    src/UndoFramework/lineaction.cpp
    src/AutomaticRoomIdentification/roomdefinition.cpp
    src/AutomaticRoomIdentification/roomidentification.cpp

    src/global.cpp
        )
# all header files that should be treated with moc
set( HDR
## Controllers
    src/mainWindow.h
    src/GraphicView.h
    src/graphicscene.h

    src/widgets/roomlistwidget.h
    src/widgets/propertywidget.h
    src/widgets/transitionwidget.h
    src/widgets/layerwidget.h
    src/widgets/templatewidget.h
    src/widgets/runningwidget.h
    src/widgets/roomwidget.h
    src/widgets/widgetlandmark.h
    src/widgets/widgetsettings.h
    src/widgets/settingdialog.h
    src/widgets/inifilewidget.h
    src/widgets/snappingoptions.h
    src/widgets/sourcewidget.h
    src/widgets/goalwidget.h

## JPSelements
    src/jpsgoal.h
    src/jpszone.h
    src/datamanager.h
    src/jpscrossing.h
    src/jpsLineItem.h
    src/jpstransition.h
    src/jpsobstacle.h
    src/jpslandmark.h
    src/jpstrack.h
    src/jpsconnection.h
    src/jpsregion.h
    src/jpssource.h

## Models
    src/models/jpselementlistmodel.h
    src/models/jpssourcelistmodel.h
    src/models/layer.h
    src/models/jpsgoallistmodel.h

## XML
    src/XML/goalreader.h
    src/XML/sourcereader.h

## MISC
    dxflib/src/dl_writer_ascii.h
    dxflib/src/dl_writer.h
    dxflib/src/dl_global.h
    dxflib/src/dl_extrusion.h
    dxflib/src/dl_exception.h
    dxflib/src/dl_entities.h
    dxflib/src/dl_dxf.h
    dxflib/src/dl_creationinterface.h
    dxflib/src/dl_creationadapter.h
    dxflib/src/dl_codes.h
    dxflib/src/dl_attributes.h

    src/UndoFramework/action.h
    src/UndoFramework/actionstack.h
    src/UndoFramework/lineaction.h
    src/AutomaticRoomIdentification/roomID.h
    src/AutomaticRoomIdentification/roomdefinition.h
    src/AutomaticRoomIdentification/roomidentification.h

    src/elementtype.h
    src/global.h
        )

# *.ui files
set( UIS
    forms/mainwindow.ui
    forms/roomwidget.ui
    forms/widgetlandmark.ui
    forms/widgetsettings.ui
    forms/settingdialog.ui
    forms/inifilewidget.ui
    forms/snappingoptions.ui
    forms/sourcewidget.ui
    forms/goalwidget.ui
    forms/roomlistwidget.ui
    forms/propertywidget.ui
    forms/transitionwidget.ui
    forms/layerwidget.ui
    forms/templatewidget.ui
    forms/runningwidget.ui
  )

# and finally a resource file
set( RCS
        Resources/resources.qrc
  )

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

IF (WIN32)
    SET (QT_USE_QTMAIN TRUE)
ENDIF (WIN32)

set(DESIRED_QT_VERSION 5)

message(STATUS "Searching for Qt version ...................... " ${DESIRED_QT_VERSION})
# find packages according to qt version
if("${DESIRED_QT_VERSION}" STREQUAL "5")
  FIND_PACKAGE(Qt5Widgets REQUIRED)
  FIND_PACKAGE(Qt5Xml REQUIRED)
  #-----------------------------------------------------
  set(QT5_INSTALLED TRUE)
  set(QT_USE_QTXML TRUE)
  set(QTVERSION 5)
  MESSAGE(STATUS "Found Qt version .............................. ${Qt5Core_VERSION_STRING}")
endif()

execute_process(COMMAND ${QT_QMAKE_EXECUTABLE} -query QT_VERSION OUTPUT_VARIABLE QT_VERSION)
# qt version > 5 are not supported
if(${QTVERSION} VERSION_LESS 5)
  message(FATAL_ERROR "QT version " ${QTVERSION} " not supported. Try 5")
endif()

## Set-up icon of app
if(APPLE)
  ADD_DEFINITIONS(-DQT_NO_CAST_TO_ASCII)
  # include the icns file in the target
  SET(SRCS ${SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/Resources/jpseditor.icns)
  SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/Resources/jpseditor.icns PROPERTIES MACOSX_PACKAGE_LOCATION
          Resources)
  elseif(WIN32)
  SET(SRCS ${SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/Resources/jpseditor.rc)
endif()

# this command will generate rules that will run rcc on all files from RCS
# in result RC_SRCS variable will contain paths to files produced by rcc
if( ${QT5_INSTALLED})
  message(STATUS "Working with Qt5")
  # http://www.kdab.com/using-cmake-with-qt-5/
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}" )

  qt5_wrap_ui( UI_HDRS ${UIS} )
  qt5_add_resources( RCS ${RCS} )
  #qt5_use_package(JPSeditor Widgets)

  add_executable( ${PROJECT_NAME} MACOSX_BUNDLE WIN32
  ${SRCS}
  ${MOC_HDRS}
  ${UI_HDRS}
  ${RCS}
  )
 qt5_use_modules(${PROJECT_NAME} Widgets)
 target_link_libraries(${PROJECT_NAME} ${Qt5Widgets_LIBRARIES} Qt5::Xml)
else()
  message(FATAL_ERROR "QT NOT FOUND - ABORT")
endif()

SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
#----------------------------------------------------------------------
# enable as many warnings as possible
set(warning_flags "-Wall -Wextra")
# issue all the warnings demanded by strict iso c and iso c++
set(warning_flags "${warning_flags} -pedantic")
#---------------------------- compiler -------------------------------
# Initialize CXXFLAGS.
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} ${warning_flags}")
  set(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} ${warning_flags} -O0 -g -Wunused-variable -Wunused-parameter -Wunused-function -Wshadow -Wunused")
endif()


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(STATUS "Using compiler ................................ Clang (${CMAKE_CXX_COMPILER_VERSION})")
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
if(CMAKE_COMPILER_IS_GNUCXX)
    message( STATUS "Using compiler ................................ g++ (${CMAKE_CXX_COMPILER_VERSION}")
endif(CMAKE_COMPILER_IS_GNUCXX)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_CXX_LDFLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} -DTRACE_LOGGING")
  message(STATUS "Debug flags: "  ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_DEBUG} )
  message(STATUS "Debug flags ..................................."  ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_DEBUG} )
else(CMAKE_BUILD_TYPE MATCHES Release)
  message(STATUS "Release flags ................................." ${CMAKE_CXX_FLAGS} " ,  " ${CMAKE_CXX_FLAGS_RELEASE} )
endif(CMAKE_BUILD_TYPE MATCHES Debug)

# PGI ++ Flags
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "PGI")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --c++11 -Minform=inform")
endif()
#--------------------------------------------------------------------------------
# Now the installation stuff below
#--------------------------------------------------------------------------------
INSTALL(TARGETS ${PROJECT_NAME}
  BUNDLE DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT Runtime
  RUNTIME DESTINATION bin COMPONENT Runtime
  # PERMISSIONS OWNER_EXECUTE 600
  )
set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "${BIN_INSTALL_DIR}")
include(InstallRequiredSystemLibraries)

if(APPLE)
  set(EXECUTABLE "${PROJECT_NAME}.app")
elseif(WIN32)
  set(EXECUTABLE "${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}")
else()
  set(EXECUTABLE "${BIN_INSTALL_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}")
endif()

SET(plugin_dest_dir bin)
SET(qtconf_dest_dir bin)
SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}")
IF(APPLE)
  SET(plugin_dest_dir ${PROJECT_NAME}.app/Contents/MacOS)
  SET(qtconf_dest_dir ${PROJECT_NAME}/Contents/Resources)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}")
ENDIF(APPLE)

IF(WIN32)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}.exe")
ENDIF(WIN32)

if( ${QT5_INSTALLED})
  SET(DIRS ${QT_LIBRARIES})
elseif( ${QT5_INSTALLED})
  SET(DIRS ${QT_LIBRARY_DIRS})
endif()

#================================================ APPLE Stuff ======================
# Set how it shows up in the Info.plist file
IF(APPLE)
  SET(MACOSX_BUNDLE_ICON_FILE jpseditor.icns)
  SET(MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2015-2019 Forschungszentrum Juelich. All rights reserved.")
  SET(MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}")
  SET(MACOSX_BUNDLE_BUNDLE_VERSION "${JPSEDITOR_VERSION}")
  SET(MACOSX_BUNDLE_LONG_VERSION_STRING "version ${JPSEDITOR_VERSION}")
  SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${JPSEDITOR_VERSION}")
  SET(MACOSX_BUNDLE_EDIT_IDENTIFIER "https://www.jupedsim.org")
ENDIF(APPLE)

# To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
# where CPackConfig.cmake is created by including CPack
# And then there's ways to customize this as well
# https://github.com/mikemcquaid/Fabula
#set(CPACK_BINARY_DRAGNDROP ON)
#set(CPACK_GENERATOR "TBZ2")
#set(CPACK_PACKAGE_VERSION_MAJOR "${JPSEDITOR_MAJOR_VERSION}")
#set(CPACK_PACKAGE_VERSION_MINOR "${JPSEDITOR_MINOR_VERSION}")
#set(CPACK_PACKAGE_VERSION_PATCH "${JPSEDITOR_PATCH_VERSION}")
#set(CPACK_PACKAGE_VERSION "${JPSEDITOR_VERSION}")
#set(CPACK_PACKAGE_VENDOR "${JPSEDITOR_VENDOR}")
#set(CPACK_RESOURCE_FILE_README "${README_FILE}")
#if(WIN32) #
#  set(CPACK_GENERATOR "NSIS")
#  set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "${PROJECT_NAME}")
#  set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
#  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME} ${JPSEDITOR_VERSION}")
#  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/${LICENSE_FILE}")
#  set(CPACK_NSIS_EXECUTABLES_DIRECTORY "${BIN_INSTALL_DIR}")
#  #set(CPACK_NSIS_MUI_ICON "${PROJECT_ICONS_DIRECTORY}/NSIS.ico")
#  #set(CPACK_PACKAGE_ICON "${PROJECT_ICONS_DIRECTORY}\\\\NSISHeader.bmp")
#  set(CPACK_NSIS_INSTALLED_ICON_NAME "${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}")
#  #    set(CPACK_NSIS_MENU_LINKS "${LICENSE_FILE}" "License" "${README_FILE}" "Readme")
#  #   set(CPACK_NSIS_MUI_FINISHPAGE_RUN "${CPACK_NSIS_INSTALLED_ICON_NAME}")
#elseif(APPLE) #
#  set(CPACK_GENERATOR "DragNDrop")
#  set(CPACK_DMG_FORMAT "UDBZ")
#  set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
#  set(CPACK_SYSTEM_NAME "OSX")
#  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${JPSEDITOR_VERSION}")
#  #set(CPACK_PACKAGE_ICON "${ICONS_DIR}/DMG.icns")
#  #set(CPACK_DMG_DS_STORE "${ICONS_DIR}/DMGDSStore")
#  #set(CPACK_DMG_BACKGROUND_IMAGE "${ICONS_DIR}/DMGBackground.png")
#elseif(UNIX)
#  # tested with success in
#  # Linux dhcppc5 3.2.0-4-686-pae #1 SMP Debian 3.2.68-1+deb7u2 i686 GNU/Linux
#  set(CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
#  SET(CPACK_GENERATOR "DEB")
#
#  SET(CPACK_PACKAGE_DESCRIPTION "simulation core of JuPedSim")
#  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Geometry editor of JuPedSim, a framework for simulation and analysis of pedestrian dynamics")
#  #SET(CPACK_PACKAGE_VENDOR "nowardev")
#  SET(CPACK_PACKAGE_CONTACT "m.chraibi@fz-juelich.de")
#  SET(CPACK_PACKAGE_VERSION_MAJOR "${JPSEDITOR_MAJOR_VERSION}")
#  SET(CPACK_PACKAGE_VERSION_MINOR "${JPSEDITOR_MINOR_VERSION}")
#  SET(CPACK_PACKAGE_VERSION_PATCH "${JPSEDITOR_PATCH_VERSION}")
#  SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${JPSEDITOR_VERSION}")
#  SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${JPSEDITOR_VERSION}")
#  #SET(CPACK_DEBIAN_PACKAGE_DEPENDS " Boost (>= 1.59) ")
#  SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
#  SET( CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://jupedsim.org")
#  SET(CPACK_DEBIAN_PACKAGE_SUGGESTS, "jpsvis, jpsreport, jpscore")
#  SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
#  SET(CPACK_DEBIAN_PACKAGE_SECTION "science")
#  SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
#endif()
#include(CPack)
