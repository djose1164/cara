cmake_minimum_required(VERSION 3.16)

project(Cara VERSION 0.16.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 6.2 REQUIRED
    COMPONENTS Qml Quick Core Network Svg Test Charts REQUIRED)

qt_standard_project_setup(REQUIRES 6.2)
qt_policy(SET QTP0001 NEW)

add_subdirectory(src)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    add_subdirectory(tests)
endif()

# Create CaraLib target
add_library(CaraLib STATIC)

target_link_libraries(CaraLib PUBLIC
    Qt6::Qml
    Qt6::Quick
    Qt6::Core
    Qt6::Network
    Qt6::Svg
    Qt6::Test
    Qt6::Charts
    Coreplugin
    Modelsplugin
    Dataplugin
    Entitiesplugin
    Catalogueplugin
    InventoryLibplugin
    salespersonplugin
    UserLibplugin
    AuthenticationLibplugin
    UtilsLibplugin
    BillingLibplugin
    CustomerLibplugin
    ProductLibplugin
    StatisticsLibplugin
)

# Additional configurations for CaraLib
set_target_properties(CaraLib PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Create Cara executable
qt_add_executable(Cara
    main.cpp
)

qt_add_qml_module(Cara
    URI Cara
    VERSION 1.0
    RESOURCES
        assets/res.qrc
        src/qml/qml.qrc
)

qt_add_resources(Cara "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

target_link_libraries(Cara PRIVATE CaraLib)

# Additional configurations for Cara
set_target_properties(Cara PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
)

if (ANDROID)
    include(FetchContent)
    FetchContent_Declare(
      android_openssl
      DOWNLOAD_EXTRACT_TIMESTAMP true
      URL      https://github.com/KDAB/android_openssl/archive/refs/heads/master.zip
    )
    FetchContent_MakeAvailable(android_openssl)
    include(${android_openssl_SOURCE_DIR}/android_openssl.cmake)

    add_android_openssl_libraries(Cara)
endif()
