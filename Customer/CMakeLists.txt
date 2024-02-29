qt_add_library(CustomerLib STATIC)

set_source_files_properties(CustomerManager.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(CustomerLib
    URI Customer
    VERSION 1.0
    QML_FILES
        ChooseCustomer.qml
        ClientComboBox.qml
        ClientView.qml
        CustomerAPI.qml
        CustomerCard.qml
        CustomerEdit.qml
        CustomerForm.qml
        CustomerInfoBox.qml
        CustomerManager.qml
        CustomerSelector.qml
        CustomerViewItem.qml
        Customers.qml
        PersonalInformationForm.qml
        SelectedCustomer.qml
    SOURCES
        customermodel.h customermodel.cpp
        customerrepository.h customerrepository.cpp
        data/customerresource.h data/customerresource.cpp
        entity/customer.h entity/customer.cpp
        model/customerproxymodel.h model/customerproxymodel.cpp
        business/customercontroller.h business/customercontroller.cpp
    RESOURCES
        person.svg
        person-add.svg
        person-search.svg
        supervisor-account.svg
)

target_include_directories(CustomerLib PRIVATE
    data
    entity
    model
    business
    ../Salesperson
    ../utils
)
