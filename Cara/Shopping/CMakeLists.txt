cmake_minimum_required(VERSION 3.20)
project(ShoppingLib LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_add_qml_module(${PROJECT_NAME} STATIC
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
        ishoppingproductrepository.h ishoppingproductrepository.cpp
        organizationresource.h organizationresource.cpp
        productcategoryrepository.h productcategoryrepository.cpp
        productrepository.h productrepository.cpp
    DEPENDENCIES
        Cara.User Cara.Billing Cara.Utils Cara.ProductInventory Cara.Data
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ../Utils
    ../Billing
    ../authentication
    ../
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        UtilsLib
        AuthenticationLib
        UserLib
        BillingLib
        ProductInventoryLib
        DataLib
        CommonLib
)
