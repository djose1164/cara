if (NOT EMSCRIPTEN)
    find_package(Qt6 6.7 REQUIRED Concurrent)
endif()

qt_add_library(BillingLib STATIC)
add_library(Cara::Billing ALIAS BillingLib)

set_source_files_properties(BillingSettings.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(BillingLib
    URI Cara.Billing
    VERSION 1.0
    QML_FILES
        AdminBillDelegate.qml
        BillDetailsPage.qml
        BillFilterPopup.qml
        BillHomePage.qml
        BillListView.qml
        BillMessage.qml
        BillPrinter.qml
        BillViewChooser.qml
        BillingSettings.qml
        BillingSettingsItem.qml
        CommissionItemsPage.qml
        CommissionPage.qml
        CreditedView.qml
        CustomerBillDelegate.qml
        CustomerBills.qml
        CustomerBillsHome.qml
        NewPurchaseOrder.qml
        OrderQueuePage.qml
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
        ProfitProductDelegate.qml
        ProviderAPI.qml
        ProviderComboBox.qml
        ProviderForm.qml
        Providers.qml
        ProvinceComboBox.qml
        QueueDelegate.qml
        SalesPage.qml
        SalespersonBillPage.qml
    SOURCES
        bill.h bill.cpp
        billrepository.h billrepository.cpp
        business/abstractdocument.h business/abstractdocument.cpp
        business/abstractprinter.h
        business/billbusiness.h business/billbusiness.cpp
        business/filenamegenerator.h business/filenamegenerator.cpp
        business/generalprinter.h business/generalprinter.cpp
        business/ordercontroller.h business/ordercontroller.cpp
        business/orderrepository.h business/orderrepository.cpp
        business/orderresource.h business/orderresource.cpp
        sqlitecommissionrepository.h sqlitecommissionrepository.cpp
        business/pay.h business/pay.cpp
        business/paymentcontroller.h business/paymentcontroller.cpp
        business/pdforderprinter.h business/pdforderprinter.cpp
        business/purchaseorder.h business/purchaseorder.cpp
        calculator.h calculator.cpp
        commission.h commission.cpp
        commissionmodel.h commissionmodel.cpp
        commissionrepository.h
        costcalculator.h costcalculator.cpp
        data/paymentresource.h data/paymentresource.cpp
        data/restorderrepository.h data/restorderrepository.cpp
        detailsentity.h detailsentity.cpp
        ibillrepository.h ibillrepository.cpp
        model/abstractsortfiltermodel.h model/abstractsortfiltermodel.cpp
        model/billmodel.h model/billmodel.cpp
        model/billproxymodel.h model/billproxymodel.cpp
        model/ordermodel.h model/ordermodel.cpp
        orderentity.h orderentity.cpp
        orderqueue.h orderqueue.cpp
        orderqueuemodel.h orderqueuemodel.cpp
        orderstatus.h orderstatus.cpp
        ordertemplate.h ordertemplate.cpp
        payment.h payment.cpp
        profitcalculator.h profitcalculator.cpp
        queueproxymodel.h queueproxymodel.cpp
        queuerepository.h
        restcommissionrepository.h restcommissionrepository.cpp
        restsalerepository.h restsalerepository.cpp
        sale.h sale.cpp
        salemodel.h salemodel.cpp
        salerepository.h
        uniquefilenamegenerator.h uniquefilenamegenerator.cpp
        sqlitecommissionrepository.h sqlitecommissionrepository.cpp
        sqlitecommissionmodel.h sqlitecommissionmodel.cpp
    RESOURCES
        order-model.html style.css
        pdf-icon.svg
        pdf-icon-colored.svg
        orders.svg
        done_all.svg
        check_circle_fill.svg
        lists.svg
        contact-support.svg
        pending.svg
        processing.svg
        cancelled-order.png
        assigned.png
        commission.png
        order-completed.svg
        paid.png
        unpaid.png
        business.svg
        current_price.png
        previous_price.png
        assets/customer_icon.svg
    DEPENDENCIES
        QtQuick Utils Product Customer
)

target_include_directories(BillingLib
    PRIVATE
        business entity model data
        ../Customer ../Data ../authentication ../Utils ../ ../Product
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(BillingLib
    PRIVATE
        Qt6::PrintSupport
        Qt6::Network
        Qt6::Sql
        UtilsLib
        CustomerLib
        ProductLib
    INTERFACE
        ProductInventoryLib
)

if (EMSCRIPTEN)
    target_sources(BillingLib PRIVATE
        business/wasmprinter.h business/wasmprinter.cpp)
else()
    target_link_libraries(BillingLib
        PRIVATE
            Qt6::Concurrent
            UtilsLib
    )
endif()
