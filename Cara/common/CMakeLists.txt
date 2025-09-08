project(common)

qt_add_library(CommonLib STATIC)

qt_add_qml_module(CommonLib
    URI Cara.Common
    VERSION 1.0
    SOURCES
        ipersonname.h
        abstractresource.h abstractresource.cpp
        istore.h
)

target_include_directories(CommonLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
