cmake_minimum_required(VERSION 3.20)
project(CaraLib LANGUAGES CXX)

set(CMAKE_AUTORCC ON)

add_library(LoginHelper STATIC
    loginhelper.h loginhelper.cpp)

target_link_libraries(LoginHelper
    PRIVATE Qt6::Core Qt6::Qml Qt6::Test AuthenticationLib UserLib UtilsLib CommonLib)

target_include_directories(LoginHelper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

qt_add_library(EnvHelper STATIC
    dotenv.h)

qt_add_resources(LoginHelper "LoginHelper_env"
    FILES .env)

qt_add_library(${PROJECT_NAME} STATIC)

qt_add_qml_module(${PROJECT_NAME}
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
        cara-mobile-icon.ico
        cara-mobile-icon.png
    SOURCES
        config.h.in
        healthresource.h healthresource.cpp
        restservice.h restservice.cpp
)

set(CARA_VERSION ${CMAKE_PROJECT_VERSION})
string(TIMESTAMP TODAY "%d/%m/%Y")
set(CARA_BUILD_TIME ${TODAY})
configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Qml
    Qt6::Quick
    Qt6::Core
    Qt6::Network
    Qt6::Svg
    AuthenticationLibplugin
    BillingLibplugin
    CustomerLibplugin
    DataLibplugin
    ReviewLibplugin
    ShoppingLibplugin
    StatisticsLibplugin
    UserLibplugin
    UtilsLibplugin
    SalespersonLibplugin
    ControlsLibplugin
    CommonLibplugin
)

target_include_directories(CaraLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Utils)
add_subdirectory(Data)
add_subdirectory(Network)
add_subdirectory(Controls)
add_subdirectory(Statistics)
add_subdirectory(Authentication)
add_subdirectory(Shopping)
add_subdirectory(ProductInventory)
add_subdirectory(Salesperson)
add_subdirectory(User)
add_subdirectory(Billing)
add_subdirectory(Customer)
add_subdirectory(Review)
add_subdirectory(common)
