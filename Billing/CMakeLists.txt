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
        BillListView.qml
        BillMessage.qml
        BillPage.qml
        BillPageHeader.qml
        BillPrinter.qml
        BillViewChooser.qml
        BillingSettings.qml
        BillingSettingsItem.qml
        CreditedView.qml
        CustomerBillDelegate.qml
        CustomerBills.qml
        CustomerBillsHome.qml
        NewPurchaseOrder.qml
        OrderQueue.qml
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
        ProductSale.qml
    SOURCES
        billrepository.h billrepository.cpp
        business/abstractdocument.h business/abstractdocument.cpp
        business/billbusiness.h business/billbusiness.cpp
        business/filenamegenerator.h business/filenamegenerator.cpp
        business/ordercontroller.h business/ordercontroller.cpp
        business/orderrepository.h business/orderrepository.cpp
        business/orderresource.h business/orderresource.cpp
        business/pay.h business/pay.cpp
        business/paymentcontroller.h business/paymentcontroller.cpp
        business/pdforderprinter.h business/pdforderprinter.cpp
        business/purchaseorder.h business/purchaseorder.cpp
        costcalculator.h costcalculator.cpp
        data/paymentresource.h data/paymentresource.cpp
        data/restorderrepository.h data/restorderrepository.cpp
        detailsentity.h detailsentity.cpp
        document.h document.cpp
        entity/bill.h entity/bill.cpp
        entity/orderentity.h entity/orderentity.cpp
        entity/payment.h entity/payment.cpp
        ibillrepository.h ibillrepository.cpp
        model/billmodel.h model/billmodel.cpp
        model/billproxymodel.h model/billproxymodel.cpp
        model/ordermodel.h model/ordermodel.cpp
        orderqueueconstants.h
        orderstatus.h orderstatus.cpp
        ordertemplate.h ordertemplate.cpp
        queueproxymodel.h queueproxymodel.cpp
        uniquefilenamegenerator.h uniquefilenamegenerator.cpp
    RESOURCES
        order-model.html style.css pdf-icon.svg orders.svg done_all.svg check_circle_fill.svg
    DEPENDENCIES
        QtQuick Utils Product Customer
)

target_include_directories(BillingLib
    PRIVATE
        business entity model data
        ../Customer ../data ../product ../authentication ../utils ../
)

find_package(Qt6 REQUIRED COMPONENTS Network)

target_link_libraries(BillingLib
    PRIVATE Qt6::Concurrent Qt6::PrintSupport Qt6::Network UtilsLib InventoryLib CustomerLib)
