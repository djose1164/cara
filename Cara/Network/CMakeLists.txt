project(NetworkLib)
find_package(Qt6 REQUIRED COMPONENTS Core Network Quick)

qt_add_library(${PROJECT_NAME} STATIC)
qt_add_qml_module(${PROJECT_NAME}
    URI Cara.Network
    SOURCES
        networkrequestsingleton.h networkrequestsingleton.cpp
        qnamsingleton.h qnamsingleton.cpp
        apienvironment.h apienvironment.cpp
        pagination.h pagination.cpp
)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Network DataLib)
