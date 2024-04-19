qt_add_library(salesperson STATIC)

set_target_properties(salesperson PROPERTIES AUTOMOC ON)

set_source_files_properties(SalespersonManager.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

set_source_files_properties(SalespersonSettings.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(salesperson
    URI Cara.Salesperson
    VERSION 1.0
    QML_FILES
        AssociateSalesperson.qml
        CreditOverview.qml
        NewSalesperson.qml
        SalespersonManager.qml
        SalespersonSettings.qml
        delegate/AssociateSalespersonDelegate.qml
    RESOURCES
        salesperson.svg
    SOURCES
        salespersoncontroller.h salespersoncontroller.cpp
        data/salespersonrepository.h data/salespersonrepository.cpp
        data/salespersonrepositoryimp.h data/salespersonrepositoryimp.cpp
        salesperson.h salesperson.cpp
        salespersonmodel.h salespersonmodel.cpp
    DEPENDENCIES
        User
)

target_include_directories(salesperson
    PRIVATE business data ../data ../user ../utils
)
