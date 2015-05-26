# ------------------------- Begin Generic CMake Variable Logging ------------------

message(STATUS  "The system is: ${CMAKE_SYSTEM_NAME} - ${CMAKE_SYSTEM_VERSION} - ${CMAKE_SYSTEM_PROCESSOR}")
# the compiler used for C files 
MESSAGE( STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER} )

# the compiler used for C++ files 
MESSAGE( STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER} )
# ------------------------- End of Generic CMake Variable Logging ------------------
cmake_minimum_required(VERSION 2.8)

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/" ${CMAKE_MODULE_PATH})

# set project's name
project( JuPedSim )

#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")


#------------------ set important directories --------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")
# Debug messages
message( STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR} )
message( STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR} )
message( STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR} )
message( STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
message( STATUS "EXECUTABLE_OUTPUT_PATH: " ${EXECUTABLE_OUTPUT_PATH} )
message( STATUS "CMAKE_VERBOSE_MAKEFILE: " ${CMAKE_VERBOSE_MAKEFILE} )

message (STATUS "MODULE_PATH: " ${CMAKE_MODULE_PATH})
set(CMAKE_COLOR_MAKEFILE ON)
set(JPSGUI_MAJOR_VERSION 0)
set(JPSGUI_MINOR_VERSION 1)
set(JPSGUI_PATCH_VERSION 0)
set(JPSGUI_VERSION
  ${JPSGUI_MAJOR_VERSION}.${JPSGUI_MINOR_VERSION}.${JPSGUI_PATCH_VERSION})
message( STATUS "JPSGUI_VERSION: " ${JPSGUI_VERSION} )

# we need this to be able to include headers produced by uic in our code
# (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})





set( SAMPLE_SRCS
    src/mainWindow.cpp
    src/main.cpp 
    src/GraphicView.cpp
    src/roomwidget.cpp
    src/rooms.cpp
    src/datamanager.cpp  
    src/jpscrossing.cpp  
    src/jpsLineItem.cpp  
    src/jpsexit.cpp
    src/jpsobstacle.cpp 
	src/jpslandmark.cpp
	src/jpswaypoint.cpp
	src/widgetlandmark.cpp
    dxflib/src/dl_writer_ascii.cpp
    dxflib/src/dl_dxf.cpp
  )  
# all header files that should be treated with moc
set( SAMPLE_HDR
    src/mainWindow.h 
    src/GraphicView.h
    src/roomwidget.h
    src/rooms.h
    src/datamanager.h  
    src/jpscrossing.h  
    src/jpsLineItem.h  
    src/jpsexit.h     
    src/jpsobstacle.h
	src/jpslandmark.h
	src/jpswaypoint.h
	src/widgetlandmark.h
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
 
  )

# *.ui files
set( SAMPLE_UIS
    forms/mainwindow.ui
    forms/roomwidget.ui
	forms/widgetlandmark.ui
  )

# and finally a resource file
set( SAMPLE_RCS
    forms/Ressource.qrc
  )


set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

#--------------- here comes QT. Version problem (4 or 5?)

FIND_PROGRAM(QT_QMAKE_EXECUTABLE NAMES qmake  REQUIRED)
FIND_PACKAGE(Qt COMPONENTS QtXml QT_USE_QTNETWORK QtXmlPatterns REQUIRED)

if("${DESIRED_QT_VERSION}" STREQUAL "")
  set(DESIRED_QT_VERSION "5")
endif()

if( ${DESIRED_QT_VERSION} VERSION_GREATER "5" OR ${DESIRED_QT_VERSION} VERSION_LESS "3")
  message(WARNING "QT version: " ${DESIRED_QT_VERSION} " not supported")
endif()

message(STATUS "DESIRED_QT_VERSION " ${DESIRED_QT_VERSION})
message(STATUS "QT5_INSTALLED: " ${QT5_INSTALLED})
message(STATUS "QT4_INSTALLED: " ${QT4_INSTALLED})
message(STATUS "QT3_INSTALLED: " ${QT3_INSTALLED})

# by default only QtCore and QtGui modules are enabled
# other modules must be enabled like this:
set(QT_USE_QTXML TRUE)  # needed!
set(QT_USE_QTNETWORK TRUE)

# this command will generate rules that will run rcc on all files from SAMPLE_RCS
# in result SAMPLE_RC_SRCS variable will contain paths to files produced by rcc
if( ${QT4_INSTALLED} )
  message(STATUS "Working with Qt4")
  QT4_ADD_RESOURCES( SAMPLE_RC_SRCS ${SAMPLE_RCS} )
  # and finally this will run moc:
  QT4_WRAP_CPP( SAMPLE_MOC_HDRS ${SAMPLE_MOC_HDRS} )
  # this will run uic on .ui files:
  QT4_WRAP_UI( SAMPLE_UI_HDRS ${SAMPLE_UIS} )
  QT4_ADD_RESOURCES(SAMPLE_RESOURCES_RCS ${SAMPLE_RCS})


  include( ${QT_USE_FILE} )
elseif( ${QT5_INSTALLED} )
  message(STATUS "Working with Qt5")	
  find_package(Qt5Widgets REQUIRED)
  qt5_wrap_ui(SAMPLE_UI_HDRS ${SAMPLE_UIS})
endif() 

IF(APPLE)
  SET(MACOSX_BUNDLE_ICON_FILE jupedsim.icns)
  SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/icons/jupedsim.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  # include the icns file in the target
  SET(SAMPLE_SRCS ${SAMPLE_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/icons/jupedsim.icns)
ENDIF(APPLE)

# here we instruct CMake to build JPSgui executable from all of the source files
ADD_EXECUTABLE( JuPedSim MACOSX_BUNDLE WIN32
  ${SAMPLE_SRCS}
  ${SAMPLE_MOC_HDRS}
  ${SAMPLE_UI_HDRS} 
  ${SAMPLE_RESOURCES_RCS}
  )

# QVTK is for VTK 5.x. For VTK 6, use ${VTK_LIBRARIES} instead
if(VTK_LIBRARIES)
  if(${VTK_VERSION} VERSION_LESS "6")
    TARGET_LINK_LIBRARIES( JuPedSim ${QT_LIBRARIES} QVTK)
  else()        
    TARGET_LINK_LIBRARIES( JuPedSim  ${VTK_LIBRARIES} ${QT_LIBRARIES})
  endif()
else() # this else is not yet tested
  target_link_libraries(JuPedSim vtkHybrid QVTK vtkViews ${QT_LIBRARIES})
endif() 


SET_TARGET_PROPERTIES( JuPedSim PROPERTIES MACOSX_BUNDLE TRUE)
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
  #    set(CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

#--------------------
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
message(STATUS "Checking for C++11 compiler - available")

#---------------------

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  message(STATUS "Using Clang " ${CMAKE_CXX_COMPILER_VERSION})
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
if(CMAKE_COMPILER_IS_GNUCXX)
  message( STATUS "Using g++ " ${CMAKE_CXX_COMPILER_VERSION})
endif(CMAKE_COMPILER_IS_GNUCXX)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_CXX_LDFLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
  message(STATUS "Debug flags: "  ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_DEBUG} )
else(CMAKE_BUILD_TYPE MATCHES Debug)
  message(STATUS "Release flags: " ${CMAKE_CXX_FLAGS} " ,  " ${CMAKE_CXX_FLAGS_RELEASE} )
endif(CMAKE_BUILD_TYPE MATCHES Debug)

# PGI ++ Flags
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "PGI")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --c++11 -Minform=inform")	
endif()


#--------------------------------------------------------------------------------
# Now the installation stuff below
#--------------------------------------------------------------------------------
SET(plugin_dest_dir bin)
SET(qtconf_dest_dir bin)
SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/JuPedSim")
IF(APPLE)
  SET(plugin_dest_dir JuPedSim.app/Contents/MacOS)
  SET(qtconf_dest_dir JuPedSim.app/Contents/Resources)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/JuPedSim.app")  
ENDIF(APPLE)

IF(WIN32)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/JuPedSim.exe")
ENDIF(WIN32)

INSTALL(TARGETS JuPedSim
  BUNDLE DESTINATION . COMPONENT Runtime
  RUNTIME DESTINATION bin COMPONENT Runtime
  
  )

#--------------------------------------------------------------------------------
# Install needed Qt plugins by copying directories from the qt installation
# One can cull what gets copied by using 'REGEX "..." EXCLUDE'
INSTALL(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION ${plugin_dest_dir}/plugins COMPONENT Runtime)

#--------------------------------------------------------------------------------
# install a qt.conf file
# this inserts some cmake code into the install script to write the file
INSTALL(CODE "
    file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"\")
    " COMPONENT Runtime)


#--------------------------------------------------------------------------------
# Use BundleUtilities to get all other dependencies for the application to work.
# It takes a bundle or executable along with possible plugins and inspects it
# for dependencies.  If they are not system dependencies, they are copied.

# directories to look for dependencies
SET(DIRS ${QT_LIBRARY_DIRS}) #  ${VTK_LIBRARIES}

# Now the work of copying dependencies into the bundle/package
# The quotes are escaped and variables to use at install time have their $ escaped
# An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
# Note that the image plugins depend on QtSvg and QtXml, and it got those copied
# over.
INSTALL(CODE "
    file(GLOB_RECURSE QTPLUGINS
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
    include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
    " COMPONENT Runtime)




#================================================ APPLE Stuff ======================

IF(APPLE)
  # set how it shows up in the Info.plist file
  SET(MACOSX_GUI_ICON jupedsim.icns)
  SET(MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2015 Forschungszentrum Juelich. All rights reserved.")
  SET(MACOSX_BUNDLE_BUNDLE_NAME "JuPedSim")
  SET(MACOSX_BUNDLE_BUNDLE_VERSION "${JPSGUI_VERSION}")
  SET(MACOSX_BUNDLE_LONG_VERSION_STRING "version ${JPSGUI_VERSION}")
  SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${JPSGUI_VERSION}")
  SET(MACOSX_GUI_COPYRIGHT "Copyright (c) 2015 Forschungszentrum Juelich. All rights reserved.")
  SET(MACOSX_BUNDLE_GUI_IDENTIFIER "www.jupedsim.org")
  # set where in the bundle to put the icns file

ENDIF(APPLE)

# To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
# where CPackConfig.cmake is created by including CPack
# And then there's ways to customize this as well
set(CPACK_BINARY_DRAGNDROP ON)
include(CPack)


