cmake_minimum_required(VERSION 3.16)

project(Cara VERSION 0.14.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 6.2 REQUIRED
    COMPONENTS Qml Quick Core Network Svg REQUIRED)

qt_standard_project_setup(REQUIRES 6.2)
qt_policy(SET QTP0001 NEW)

add_subdirectory(core)
add_subdirectory(models)
add_subdirectory(data)
add_subdirectory(entities)
add_subdirectory(catalogue)
add_subdirectory(inventory)
add_subdirectory(salesperson)
add_subdirectory(user)
add_subdirectory(authentication)
add_subdirectory(utils)

qt_add_resources(QML_RESOURCES qml/qml.qrc)
qt_add_executable(Cara
    assets/res.qrc
    main.cpp
)

qt_add_resources(Cara "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

target_link_libraries(Cara PRIVATE
    Qt6::Qml
    Qt6::Quick
    Qt6::Core
    Qt6::Network
    Qt6::Svg
    Coreplugin
    Modelsplugin
    Dataplugin
    Entitiesplugin
    Catalogueplugin
    Inventoryplugin
    salespersonplugin
    UserLibplugin
    AuthenticationLibplugin
    UtilsLibplugin
)

# Link the resources to the executable
target_sources(Cara PRIVATE ${QML_RESOURCES})

# Additional configurations
set_target_properties(Cara PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
)


include(android_openssl/android_openssl.cmake)
if (ANDROID)
    add_android_openssl_libraries(Cara)
endif()
