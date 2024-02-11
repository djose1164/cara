add_library(StoreLib STATIC)
qt_add_qml_module(StoreLib
    URI Store
    SOURCES
        istore.h istore.cpp
)

target_link_libraries(StoreLib
    PRIVATE Qt6::Qml)
