cmake_minimum_required(VERSION 3.20)
project(ShoppingLib LANGUAGES CXX)

qt_add_library(${PROJECT_NAME} STATIC)
qt_add_qml_module(${PROJECT_NAME}
    URI Cara.Shopping
    VERSION 1.0
    QML_FILES
        Catalogue.qml
        CategoryComboBox.qml
        LikeButton.qml
        PriceSpinBox.qml
        ProductItem.qml
        ShopProduct.qml
        StatusPopup.qml
    RESOURCES
        catalogue.svg
        favorite.svg
        favorite_fill.svg
    SOURCES
        cartcontroller.h cartcontroller.cpp
        cataloguerepository.h cataloguerepository.cpp
        favorite.h favorite.cpp
        favoriteproduct.h favoriteproduct.cpp
        iproductcategoryrepository.h iproductcategoryrepository.cpp
        iproductrepository.h iproductrepository.cpp
        organizationresource.h organizationresource.cpp
        productcategoryrepository.h productcategoryrepository.cpp
        productrepository.h productrepository.cpp
    DEPENDENCIES
        User Billing Utils Product
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ../data
    ../utils
    ../user
    ../billing
    ../product
)

target_link_libraries(${PROJECT_NAME} PRIVATE UtilsLib)
