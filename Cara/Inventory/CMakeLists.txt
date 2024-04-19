qt_add_library(InventoryLib STATIC)
qt_add_qml_module(InventoryLib
    URI Cara.Inventory
    VERSION 1.0
    QML_FILES
        InventoryForm.qml
        ProductSelector.qml
        ProductSelectorDelegate.qml
        SelectedProducts.qml
        ProductViewItem.qml
    SOURCES
        iinventoryrepository.h iinventoryrepository.cpp
        inventory.h inventory.cpp
        inventoryrepository.h inventoryrepository.cpp
        model/productmodel.h model/productmodel.cpp
        model/productproxymodel.h model/productproxymodel.cpp
    DEPENDENCIES
        Product
)

target_include_directories(InventoryLib
    PRIVATE model ../product ../utils)
