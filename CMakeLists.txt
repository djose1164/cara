cmake_minimum_required(VERSION 3.16)

project(Cara VERSION 0.13.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTORCC ON)

find_package(Qt6 6.2 REQUIRED
    COMPONENTS Qml Quick Core Network Svg QuickControls2 REQUIRED)

qt_standard_project_setup(REQUIRES 6.2)

add_subdirectory(core)
add_subdirectory(models)
add_subdirectory(data)
add_subdirectory(entities)
add_subdirectory(catalogue)

qt_add_executable(Cara
    qml/qml.qrc
    assets/res.qrc
    main.cpp
)

target_link_libraries(Cara PRIVATE
    Qt6::Qml
    Qt6::Quick
    Qt6::Core
    Qt6::Network
    Qt6::Svg
    Qt6::QuickControls2
    Coreplugin
    Modelsplugin
    Dataplugin
    Entitiesplugin
    Catalogueplugin
)

# Link the resources to the executable
target_sources(Cara PRIVATE ${QT_RESOURCES})

# Additional configurations
set_target_properties(Cara PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

include(FetchContent)

if (ANDROID)
    FetchContent_Declare(
      android_openssl
      DOWNLOAD_EXTRACT_TIMESTAMP true
      URL      https://github.com/KDAB/android_openssl/archive/refs/heads/master.zip
    )
    FetchContent_MakeAvailable(android_openssl)
    include(${android_openssl_SOURCE_DIR}/android_openssl.cmake)
endif()
