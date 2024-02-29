qt_add_library(ProductLib STATIC)

set_source_files_properties(ProductManager.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(ProductLib
    URI Product
    VERSION 1.0
    QML_FILES
        AddProductView.qml
        Product.qml
        ProductApi.qml
        ProductCard.qml
        ProductComboBox.qml
        ProductDelegate.qml
        ProductEntry.qml
        ProductImage.qml
        ProductPage.qml
        ProductRequest.qml
        ProductStocks.qml
        ProductThumbnail.qml
        ProductView.qml
        StockPopup.qml
        ProductForm.qml
        ProductManager.qml
)

target_include_directories(ProductLib PRIVATE business ../inventory)
