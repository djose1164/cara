qt_add_library(Inventory STATIC)
qt_add_qml_module(Inventory
    URI inventory
    VERSION 1.0
    QML_FILES
        InventoryForm.qml
    RESOURCES
        
    SOURCES
        iinventoryrepository.h iinventoryrepository.cpp
        inventoryrepository.h inventoryrepository.cpp
)


