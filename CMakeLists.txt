cmake_minimum_required(VERSION 3.20)

project(Cara VERSION 0.18.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.7 REQUIRED
    COMPONENTS Qml Quick Core Network Svg Test Concurrent PrintSupport Graphs REQUIRED)

qt_standard_project_setup(REQUIRES 6.7)
qt_policy(SET QTP0001 NEW)

add_subdirectory(Data)
add_subdirectory(Shopping)
add_subdirectory(Inventory)
add_subdirectory(Salesperson)
add_subdirectory(User)
add_subdirectory(Authentication)
add_subdirectory(Utils)
add_subdirectory(Billing)
add_subdirectory(Customer)
add_subdirectory(Product)
add_subdirectory(Statistics)
add_subdirectory(Store)
add_subdirectory(Review)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    #add_subdirectory(tests)
endif()

if (WIN32)
    qt_add_executable(Cara
        main.cpp cara.rc
    )
else()
    qt_add_executable(Cara
        main.cpp
    )
endif()

# Create CaraLib target
add_library(CaraLib STATIC)

qt_add_qml_module(Cara
    URI Cara
    VERSION 1.0
    QML_FILES
        Main.qml
        MainFooter.qml
        MainPage.qml
        SidePane.qml
        SettingsPage.qml
        ContactUs.qml
    RESOURCES
        assets/res.qrc
        qml/qml.qrc
        twitter.svg
        mail.svg
        logout.svg
        wifi-off.svg
    SOURCES
        healthresource.h healthresource.cpp
        config.h.in
)

set(CARA_VERSION ${CMAKE_PROJECT_VERSION})
configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

target_link_libraries(CaraLib PUBLIC
    Qt6::Qml
    Qt6::Quick
    Qt6::Core
    Qt6::Network
    Qt6::Svg
    Qt6::Test
    Dataplugin
    ShoppingLibplugin
    InventoryLibplugin
    salespersonplugin
    UserLibplugin
    AuthenticationLibplugin
    UtilsLibplugin
    BillingLibplugin
    CustomerLibplugin
    ProductLibplugin
    StatisticsLibplugin
    ReviewLibplugin
)

target_link_libraries(Cara PRIVATE CaraLib)

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
