qt_add_library(ProductLib STATIC)

set_source_files_properties(ProductManager.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(ProductLib
    URI Cara.Product
    VERSION 1.0
    QML_FILES
        AddProductPage.qml
        AddProductView.qml
        EditProductPage.qml
        PriceHistoryPage.qml
        ProductApi.qml
        ProductCard.qml
        ProductComboBox.qml
        ProductDelegate.qml
        ProductEntry.qml
        ProductForm.qml
        ProductImage.qml
        ProductManager.qml
        ProductPage.qml
        ProductRequest.qml
        ProductThumbnail.qml
        ProductView.qml
        product_helper.js
    SOURCES
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
        Cara.Network
)

target_include_directories(ProductLib
    PRIVATE ..
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ProductLib PUBLIC NetworkLib DataLib UtilsLib)
