qt_add_library(Core STATIC)
qt_add_qml_module(Core
    URI core
    VERSION 1.0
    SOURCES
        billbusiness.h billbusiness.cpp
	cartbusiness.h	cartbusiness.cpp
	constants.h constants.cpp
	loginmanager.h loginmanager.cpp
	paymentbusiness.h paymentbusiness.cpp
        currentuser.h currentuser.cpp
        auth.h auth.cpp
)

include_directories(../models)
include_directories(../entities)
include_directories(../data)
