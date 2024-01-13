qt_add_library(ProductLib STATIC)
qt_add_qml_module(ProductLib
    URI Product
    VERSION 1.0
    SOURCES business/costcalculator.h business/costcalculator.cpp
)

target_include_directories(ProductLib PRIVATE business ../inventory)
