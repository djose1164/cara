qt_add_library(BillingLib STATIC)

qt_add_qml_module(BillingLib
    URI Billing
    VERSION 1.0
    QML_FILES
        NewPurchaseOrder.qml
        PaymentStatus.qml
        PaymentInput.qml
        PaymentStatusFilter.qml
        UpdatePayment.qml
        AdminBillDelegate.qml
        BillDelegate.qml
        BillDetailsPage.qml
        BillHomePage.qml
        BillMessage.qml
        BillPage.qml
        BillPageHeader.qml
        BillSearch.qml
        BillViewChooser.qml
        CustomerBillDelegate.qml
        CustomerBills.qml
        CustomerBillsHome.qml
        BillFilterPopup.qml
    RESOURCES
        
    SOURCES
        business/orderrepository.h business/orderrepository.cpp
        business/ordercontroller.h business/ordercontroller.cpp
        data/restorderrepository.h data/restorderrepository.cpp
    DEPENDENCIES
        QtQuick
)

target_include_directories(BillingLib PRIVATE business ../user ../data)
