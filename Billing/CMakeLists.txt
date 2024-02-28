qt_add_library(BillingLib STATIC)

set_source_files_properties(BillingSettings.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
)

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
        BillingSettings.qml
        BillingSettingsItem.qml
        CreditedView.qml
        CustomerBillDelegate.qml
        CustomerBills.qml
        CustomerBillsHome.qml
        NewPurchaseOrder.qml
        OrderState.qml
        OrderStatusCB.qml
        PayItem.qml
        PaySuccess.qml
        PaymentDetails.qml
        PaymentDetailsPage.qml
        PaymentInput.qml
        PaymentStatus.qml
        PaymentStatusFilter.qml
        PdfBillListing.qml
    SOURCES
        billrepository.h billrepository.cpp
        business/abstractdocument.h business/abstractdocument.cpp
        business/billbusiness.h business/billbusiness.cpp
        business/cartbusiness.h	business/cartbusiness.cpp
        business/filenamegenerator.h business/filenamegenerator.cpp
        business/ordercontroller.h business/ordercontroller.cpp
        business/orderrepository.h business/orderrepository.cpp
        business/orderresource.h business/orderresource.cpp
        business/pay.h business/pay.cpp
        business/paymentcontroller.h business/paymentcontroller.cpp
        business/pdforderprinter.h business/pdforderprinter.cpp
        business/purchaseorder.h business/purchaseorder.cpp
        data/paymentresource.h data/paymentresource.cpp
        data/restorderrepository.h data/restorderrepository.cpp
        document.h document.cpp
        entity/bill.h entity/bill.cpp
        entity/orderentity.h entity/orderentity.cpp
        entity/payment.h entity/payment.cpp
        ibillrepository.h ibillrepository.cpp
        model/billmodel.h model/billmodel.cpp
        model/billproxymodel.h model/billproxymodel.cpp
        model/ordermodel.h model/ordermodel.cpp
        ordertemplate.h ordertemplate.cpp
        uniquefilenamegenerator.h uniquefilenamegenerator.cpp
    RESOURCES
    order-model.html style.css pdf.svg pdf-icon.svg
    DEPENDENCIES
        QtQuick Utils Entities
        QML_FILES BillPrinter.qml
)

target_include_directories(BillingLib
    PRIVATE
        business entity model data
        ../user ../data ../entities ../authentication ../utils ../
)

target_link_libraries(BillingLib
    PRIVATE Qt6::Concurrent Qt6::PrintSupport Entities UtilsLib)
