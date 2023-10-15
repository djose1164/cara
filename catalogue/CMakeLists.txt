qt_add_library(Catalogue STATIC)
qt_add_qml_module(Catalogue
    URI catalogue
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

include_directories(../entities)
include_directories(../core)
include_directories(../data)
