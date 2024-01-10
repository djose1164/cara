qt_add_library(BillingLib STATIC)

qt_add_qml_module(BillingLib
    URI Billing
    VERSION 1.0
    QML_FILES
        NewPurchaseOrder.qml

    RESOURCES
        
    SOURCES
        business/orderrepository.h business/orderrepository.cpp
        business/ordercontroller.h business/ordercontroller.cpp
        data/restorderrepository.h data/restorderrepository.cpp
    DEPENDENCIES
        QtQuick
)

target_include_directories(BillingLib PRIVATE business ../user ../data)
