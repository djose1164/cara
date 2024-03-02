qt_add_library(salesperson STATIC)

set_target_properties(salesperson PROPERTIES AUTOMOC ON)

set_source_files_properties(SalespersonManager.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

set_source_files_properties(SalespersonSettings.qml PROPERTIES
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
        SalespersonSettings.qml
    RESOURCES
        salesperson.svg
    SOURCES
        associatesalespersonservice.h associatesalespersonservice.cpp
        business/manageable.h business/manageable.cpp
        data/salespersonmanager.h data/salespersonmanager.cpp
        data/salespersonrepository.h data/salespersonrepository.cpp
        data/salespersonrepositoryimp.h data/salespersonrepositoryimp.cpp
        salesperson.h salesperson.cpp
        salespersoncontroller.h salespersoncontroller.cpp
        salespersonmodel.h salespersonmodel.cpp
    DEPENDENCIES
        User
)

target_include_directories(salesperson
    PRIVATE business data ../data ../user ../utils
)
