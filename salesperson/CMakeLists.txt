qt_add_library(associate_salesperson STATIC)

set_target_properties(associate_salesperson PROPERTIES AUTOMOC ON)

qt_add_qml_module(associate_salesperson
    URI AssociateSalesperson
    VERSION 1.0
    QML_FILES
        AssociateSalesperson.qml
        NewSalesperson.qml
        delegate/AssociateSalespersonDelegate.qml
    RESOURCES
        salesperson.svg
    SOURCES
        associatesalespersonservice.h associatesalespersonservice.cpp
        data/associatesalespersonrepository.h data/associatesalespersonrepository.cpp
        entity/salesperson.h entity/salesperson.cpp
        data/associatesalespersonrepositoryimp.h data/associatesalespersonrepositoryimp.cpp
)

include_directories(../data)
