qt_add_library(BillingLib STATIC)

qt_add_qml_module(BillingLib
    URI Billing
    VERSION 1.0
    QML_FILES
        AdminBillDelegate.qml
        BillDetailsPage.qml
        BillFilterPopup.qml
        BillHomePage.qml
        BillMessage.qml
        BillPage.qml
        BillPageHeader.qml
        BillViewChooser.qml
        CreditedView.qml
        CustomerBillDelegate.qml
        CustomerBills.qml
        CustomerBillsHome.qml
        NewPurchaseOrder.qml
        PayItem.qml
        PaySuccess.qml
        PaymentDetails.qml
        PaymentDetailsPage.qml
        PaymentInput.qml
        PaymentStatus.qml
        PaymentStatusFilter.qml
    SOURCES
        billrepository.h billrepository.cpp
        business/billbusiness.h business/billbusiness.cpp
        business/cartbusiness.h	business/cartbusiness.cpp
        business/ordercontroller.h business/ordercontroller.cpp
        business/orderrepository.h business/orderrepository.cpp
        business/pay.h business/pay.cpp
        business/paymentcontroller.h business/paymentcontroller.cpp
        business/purchaseorder.h business/purchaseorder.cpp
        data/paymentresource.h data/paymentresource.cpp
        data/restorderrepository.h data/restorderrepository.cpp
        entity/bill.h entity/bill.cpp
        entity/orderentity.h entity/orderentity.cpp
        entity/payment.h entity/payment.cpp
        ibillrepository.h ibillrepository.cpp
        model/billmodel.h model/billmodel.cpp
        model/billproxymodel.h model/billproxymodel.cpp
        model/ordermodel.h model/ordermodel.cpp
    DEPENDENCIES
        QtQuick UtilsLib Entities
)

target_include_directories(BillingLib
    PRIVATE
        business entity model data
        ../user ../data ../entities ../authentication ../utils ../
)




target_link_libraries(BillingLib PRIVATE Entities)
