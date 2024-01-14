qt_add_library(salesperson STATIC)

set_target_properties(salesperson PROPERTIES AUTOMOC ON)

set_source_files_properties(SalespersonManager.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(salesperson
    URI Salesperson
    VERSION 1.0
    QML_FILES
        AssociateSalesperson.qml
        NewSalesperson.qml
        delegate/AssociateSalespersonDelegate.qml
        SalespersonManager.qml
    RESOURCES
        salesperson.svg
    SOURCES
        associatesalespersonservice.h associatesalespersonservice.cpp
        data/salespersonrepository.h data/salespersonrepository.cpp
        salesperson.h salesperson.cpp
        data/salespersonrepositoryimp.h data/salespersonrepositoryimp.cpp
        salespersonmodel.h salespersonmodel.cpp
        business/manageable.h business/manageable.cpp
        data/salespersonmanager.h data/salespersonmanager.cpp
        salespersoncontroller.h salespersoncontroller.cpp
)

include_directories(../data)
include_directories(../user)
