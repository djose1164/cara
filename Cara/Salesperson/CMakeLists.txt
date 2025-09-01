qt_add_library(SalespersonLib STATIC)

set_source_files_properties(SalespersonManager.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

set_source_files_properties(SalespersonSettings.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(SalespersonLib
    URI Cara.Salesperson
    VERSION 1.0
    QML_FILES
        AssociateSalesperson.qml
        CreditOverview.qml
        NewSalesperson.qml
        ProfitCalculatorPage.qml
        SalespersonManager.qml
        SalespersonSettings.qml
        delegate/AssociateSalespersonDelegate.qml
        SellerSelector.qml
    RESOURCES
        salesperson.svg
        profit_calculator.svg
    SOURCES
        abstractsellermodel.h
        data/salespersonrepository.h data/salespersonrepository.cpp
        data/salespersonrepositoryimp.h data/salespersonrepositoryimp.cpp
        salesperson.h salesperson.cpp
        salespersoncontroller.h salespersoncontroller.cpp
        sellerlistmodel.h sellerlistmodel.cpp
        sqlitesellermodel.h sqlitesellermodel.cpp
    DEPENDENCIES
        User
)

target_include_directories(SalespersonLib
    PRIVATE business data ../Data ../User ../Utils ../authentication ../
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SalespersonLib PRIVATE Qt6::Sql NetworkLib CommonLib)
