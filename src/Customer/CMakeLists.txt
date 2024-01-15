qt_add_library(CustomerLib STATIC)

set_target_properties(CustomerLib PROPERTIES AUTOMOC ON)

set_source_files_properties(CustomerManager.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(CustomerLib
    URI Customer
    VERSION 1.0
    QML_FILES
        CustomerSelector.qml
        CustomerCard.qml
        SelectedCustomer.qml
        CustomerInfoBox.qml
        CustomerViewItem.qml
        ClientComboBox.qml
        ClientView.qml
        CustomerAPI.qml
        CustomerEdit.qml
        CustomerForm.qml
        Customers.qml
        PersonalInformationForm.qml
    SOURCES
        customermodel.h customermodel.cpp
        customerrepository.h customerrepository.cpp
        data/restcustomerrepository.h data/restcustomerrepository.cpp
        entity/customer.h entity/customer.cpp
        model/customerproxymodel.h model/customerproxymodel.cpp
    RESOURCES
        person.svg
        person-add.svg
        person-search.svg
        supervisor-account.svg
)

target_include_directories(CustomerLib PRIVATE
    ./data/
    entity
    ../salesperson
    model
)
