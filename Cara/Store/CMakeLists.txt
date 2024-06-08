set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_add_library(StoreLib STATIC)
qt_add_qml_module(StoreLib
    URI Cara.Store
    SOURCES
        istore.h istore.cpp
)

target_link_libraries(StoreLib
    PRIVATE Qt6::Qml
)

target_include_directories(StoreLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
