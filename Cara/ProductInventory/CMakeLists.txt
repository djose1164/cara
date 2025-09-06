set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Sql Core)

qt_add_qml_module(ProductInventoryLib STATIC
    URI Cara.ProductInventory
    VERSION 1.0
    QML_FILES
        InventoryForm.qml
        ProductSelector.qml
        ProductSelectorDelegate.qml
        ProductViewItem.qml
        SelectedProducts.qml
        AddProductPage.qml
        EditProductPage.qml
        PriceHistoryPage.qml
        ProductCard.qml
        ProductComboBox.qml
        ProductDelegate.qml
        ProductForm.qml
        ProductImage.qml
        ProductPage.qml
        ProductRequest.qml
        ProductThumbnail.qml
        ProductView.qml
        UpdateSupplierView.qml
        PriceDialog.qml
        product_helper.js
    SOURCES
        catalogproductrepository.h catalogproductrepository.cpp
        ieditableproduct.h
        iinventoryrepository.h iinventoryrepository.cpp
        inventory.h inventory.cpp
        inventoryrepository.h inventoryrepository.cpp
        iproductrepository.h iproductrepository.cpp
        ishoppingproductrepository.h ishoppingproductrepository.cpp
        productinventorycontroller.h productinventorycontroller.cpp
        productmodel.h productmodel.cpp
        productproxymodel.h productproxymodel.cpp
        productproxymodel.h productproxymodel.cpp
        productrepository.h productrepository.cpp
        sellerproductrepositoryrest.h sellerproductrepositoryrest.cpp
        supplierproductrepository.h supplierproductrepository.cpp
        sqliteproductmodel.h sqliteproductmodel.cpp
        sqlitedatabase.h sqlitedatabase.cpp
        abstractproductmodel.h
        sqliteproductrepository.h sqliteproductrepository.cpp
        picturerepository.cpp
        picturerepository.h
        pricehistorymodel.h pricehistorymodel.cpp
        pricehistoryrepository.h
        productentity.h productentity.cpp
        productpicturerepository.h productpicturerepository.cpp
        restpricehistoryrepository.h restpricehistoryrepository.cpp
    RESOURCES
        add_a_photo.svg
        remove.svg
    DEPENDENCIES
        Cara.Network Cara.Utils Cara.Data
        SOURCES selectionproxymodel.h selectionproxymodel.cpp
)

target_link_libraries(ProductInventoryLib
    PRIVATE
        NetworkLib
        UtilsLib
        DataLib
        Qt6::Sql)
target_link_libraries(ProductInventoryLib PRIVATE Qt6::Core)

target_include_directories(ProductInventoryLib
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(tests)
endif()
