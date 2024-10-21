set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_add_qml_module(ProductInventoryLib STATIC
    URI Cara.ProductInventory
    VERSION 1.0
    QML_FILES
        InventoryForm.qml
        ProductSelector.qml
        ProductSelectorDelegate.qml
        ProductViewItem.qml
        SelectedProducts.qml
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
    DEPENDENCIES
        Cara.Product
)

target_link_libraries(ProductInventoryLib
    PRIVATE NetworkLib
     ProductLib)

target_include_directories(ProductInventoryLib
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
