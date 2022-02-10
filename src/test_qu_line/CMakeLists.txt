cmake_minimum_required(VERSION 3.1)

project(grpcclient)




set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(QT_MIN_VERSION 5.11.0)




set(OSG_DIR /home/dx/data/test/learnOsg/Depend/Debug)


find_package(Qt5 COMPONENTS Core Gui Widgets Svg OpenGL Network Xml REQUIRED)

find_package(OpenSceneGraph COMPONENTS osg osgViewer osgGA osgDB osgManipulator osgSim osgParticle osgText osgUtil osgWidget osgShadow osgFX REQUIRED)

aux_source_directory(. SRC_LIST)



include_directories(
    ${PROJECT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}
    ${OSG_DIR}/include
)


set(${PROJECT_NAME}_HDRS

    )

set(${PROJECT_NAME}_SRCS
    grpcclient.cpp
)


#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Depend/Debug/include)
#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Depend/Debug/lib)


#find_library(TESTFUNC_LIB NAMES osgd osgManipulatord osgViewerd osgDBd OpenThreadsd osgGAd HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../Depend/Debug/lib)
#find_library(TESTFUNC_LIB NAMES osgViewer HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../Depend/Debug/lib)




link_directories(${PROJECT_NAME}
    )




add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS} ${${PROJECT_NAME}_HDRS} ${SRC_LIST})





target_link_libraries(${PROJECT_NAME}
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  Qt5::OpenGL
  Qt5::Network
  Qt5::Xml
  ${OPENTHREADS_LIBRARY}
  ${OSGVIEWER_LIBRARY}
  ${OSGDB_LIBRARY}
  ${OSG_LIBRARY}
  ${OSGGA_LIBRARY}
  ${OSGMANIPULATOR_LIBRARY}
  ${OSGSIM_LIBRARY}
  ${OSGTEXT_LIBRARY}
  ${OSGUTIL_LIBRARY}
  ${OSGWIDGET_LIBRARY}
  ${OSGSHADOW_LIBRARY}
  ${OSGFX_LIBRARY}
  ${OSGPARTICLE_LIBRARY}
)


target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})




install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
