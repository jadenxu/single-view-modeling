CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
PROJECT("single_view")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})
FIND_PACKAGE(Eigen3 REQUIRED)
INCLUDE_DIRECTORIES(EIGEN3_INCLUDE_DIR)

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

FIND_PACKAGE(OpenCV REQUIRED)
SET(CORELIBS ${QT_LIBRARIES} ${OpenCV_LIBS})

SET(single_view_SOURCES 
    main.cpp 
    my_qlabel.cpp 
    my_dialog.cpp
    my_co.cpp
    qtq.cpp
    Vrml.h
    Vrml.cpp
    Para.h
    Para.cpp)
SET(single_view_HEADERS qtq.h my_qlabel.h my_co.h my_dialog.h)
SET(single_view_FORMS qtq.ui my_dialog.ui my_co.ui)
SET(single_view_RESOURCES images.qrc)

QT4_WRAP_UI(single_view_FORMS_HEADERS ${single_view_FORMS})
QT4_WRAP_CPP(single_view_HEADERS_MOC ${single_view_HEADERS})
QT4_ADD_RESOURCES(single_view_RESOURCES_RCC ${single_view_RESOURCES})

INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

ADD_EXECUTABLE(single_view 
  ${single_view_SOURCES}
  ${single_view_HEADERS_MOC}
  ${single_view_FORMS_HEADERS}
  ${single_view_RESOURCES_RCC})
TARGET_LINK_LIBRARIES(single_view ${CORELIBS})
