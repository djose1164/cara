cmake_minimum_required(VERSION 3.20)

project(Cara VERSION 0.18.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 6.7 REQUIRED
    COMPONENTS Qml Quick Core Network Svg Test Concurrent PrintSupport Graphs QuickTest REQUIRED)

qt_standard_project_setup(REQUIRES 6.7)
qt_policy(SET QTP0001 NEW)

add_subdirectory(Cara)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    add_subdirectory(tests)
endif()

if (WIN32)
    qt_add_executable(Cara
        main.cpp
        cara.rc
        assets/res.qrc
        qml/qml.qrc
    )
else()
    qt_add_executable(Cara
        main.cpp
        assets/res.qrc
        qml/qml.qrc
    )
endif()


set(CARA_VERSION ${CMAKE_PROJECT_VERSION})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Qml
    Qt6::Quick
    Qt6::Core
    Qt6::Network
    Qt6::Svg
    Qt6::Test
    CaraLibplugin
)

qt_add_resources(Cara "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

include(GNUInstallDirs)
install(TARGETS Cara
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (ANDROID)
    set_target_properties(Cara PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
        QT_ANDROID_VERSION_NAME "${CARA_VERSION}"
        QT_ANDROID_VERSION_CODE 19
    )

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
