qt_add_library(Models STATIC)
qt_add_qml_module(Models
    URI models
    VERSION 1.0
    SOURCES
        billmodel.h billmodel.cpp
	billproxymodel.h billproxymodel.cpp
	customermodel.h customermodel.cpp
	ordermodel.h ordermodel.cpp
)

include_directories(../entities)
include_directories(../data)
