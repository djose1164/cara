qt_add_library(InventoryLib STATIC)
qt_add_qml_module(InventoryLib
    URI Inventory
    VERSION 1.0
    QML_FILES
        InventoryForm.qml
        ProductSelector.qml
        ProductSelectorDelegate.qml
        SelectedProducts.qml
    SOURCES
        iinventoryrepository.h iinventoryrepository.cpp
        inventoryrepository.h inventoryrepository.cpp
        model/productmodel.h model/productmodel.cpp
        model/productproxymodel.h model/productproxymodel.cpp
)

target_include_directories(InventoryLib
    PRIVATE model ../user ../entities ../utils)
