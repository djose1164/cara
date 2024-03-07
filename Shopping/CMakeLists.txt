qt_add_library(ShoppingLib STATIC)
qt_add_qml_module(ShoppingLib
    URI Shopping
    VERSION 1.0
    QML_FILES
        Catalogue.qml
        CategoryComboBox.qml
        StatusPopup.qml
    RESOURCES
        catalogue.svg
    SOURCES
        productrepository.h productrepository.cpp
        cataloguerepository.h cataloguerepository.cpp
        productcategoryrepository.h productcategoryrepository.cpp
        iproductrepository.h iproductrepository.cpp
        iproductcategoryrepository.h iproductcategoryrepository.cpp
)

target_include_directories(ShoppingLib PRIVATE
    ../entities
    ../data
    ../utils
)
