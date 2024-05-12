cmake_minimum_required(VERSION 3.20)
project(CaraLib LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${PROJECT_NAME} STATIC)

qt_add_qml_module(CaraLib
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
        twitter.svg
        mail.svg
        logout.svg
        wifi-off.svg
        cara-logo-v1.png
    SOURCES
        abstractresource.h abstractresource.cpp
        apienvironment.h apienvironment.cpp
        config.h.in
        healthresource.h healthresource.cpp
        networkrequestsingleton.h networkrequestsingleton.cpp
        restservice.h restservice.cpp
        qnamsingleton.h qnamsingleton.cpp
)

set(CARA_VERSION ${CMAKE_PROJECT_VERSION})
configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Qml
    Qt6::Quick
    Qt6::Core
    Qt6::Network
    Qt6::Svg
    Qt6::Test
    AuthenticationLibplugin
    BillingLibplugin
    CustomerLibplugin
    DataLibplugin
    InventoryLibplugin
    ProductLibplugin
    ReviewLibplugin
    ShoppingLibplugin
    StatisticsLibplugin
    UserLibplugin
    UtilsLibplugin
    salespersonplugin
    StoreLibplugin
    ControlsLibplugin
)

target_include_directories(CaraLib
    PRIVATE data authentication
    INTERFACE ${CMAKE_INCLUDE_CURRENT_DIR})

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
add_subdirectory(store)
add_subdirectory(Review)
add_subdirectory(Controls)
