qt_add_library(BillingLib STATIC)

qt_add_qml_module(BillingLib
    URI Billing
    VERSION 1.0
    QML_FILES
        NewPurchaseOrder.qml
        PaymentStatus.qml
        PaymentInput.qml
        PaymentStatusFilter.qml
        AdminBillDelegate.qml
        BillDetailsPage.qml
        BillHomePage.qml
        BillMessage.qml
        BillPage.qml
        BillPageHeader.qml
        BillViewChooser.qml
        CustomerBillDelegate.qml
        CustomerBills.qml
        CustomerBillsHome.qml
        BillFilterPopup.qml
        PayItem.qml
        CreditedView.qml
        PaySuccess.qml
        PaymentDetails.qml
        PaymentDetailsPage.qml
    RESOURCES
        
    SOURCES
        entity/payment.h entity/payment.cpp
        business/orderrepository.h business/orderrepository.cpp
        business/ordercontroller.h business/ordercontroller.cpp
        data/restorderrepository.h data/restorderrepository.cpp
        business/pay.h business/pay.cpp
        model/billmodel.h model/billmodel.cpp
        model/billproxymodel.h model/billproxymodel.cpp
        model/ordermodel.h model/ordermodel.cpp
        data/paymentresource.h data/paymentresource.cpp
        business/paymentcontroller.h business/paymentcontroller.cpp
    DEPENDENCIES
        QtQuick
)

target_include_directories(BillingLib
    PRIVATE business entity model data ../user ../data ../Utils ../entities
)
