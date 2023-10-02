qt_add_library(Data STATIC)
qt_add_qml_module(Data
    URI data
    VERSION 1.0
    SOURCES
        rest.h rest.cpp
)
