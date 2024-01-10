qt_add_library(Entities STATIC)
qt_add_qml_module(Entities
    URI entities
    VERSION 1.0
    SOURCES
        billentity.h billentity.cpp
	detailsentity.h detailsentity.cpp
	inventory.h inventory.cpp
	jsonexportable.h
	orderentity.h orderentity.cpp
	orderstatus.h orderstatus.cpp
	paymententity.h paymententity.cpp
	productentity.h productentity.cpp
)

target_include_directories(Entities PRIVATE ../models ../user)
