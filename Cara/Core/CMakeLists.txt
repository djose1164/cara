project(CoreLib LANGUAGES CXX)

# set(<variable> <value> CACHE <type> <docstring>)
set(API_BASE_URL http://127.0.0.1:5000 CACHE STRING "API URL to be used when issuing requests")
set(API_ENVIRONMENT production CACHE STRING "The type of API being used by this client")

qt_add_qml_module(${PROJECT_NAME} STATIC
    URI Cara.Core
    SOURCES
        types/apienv.h
        business/apienvironmentservice.h business/apienvironmentservice.cpp
        business/iapienvironmentservice.h
        ui/appstate.h ui/appstate.cpp
        #core_config.h
)

add_library(Cara::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Qml)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ui ${CMAKE_CURRENT_BINARY_DIR})

configure_file(
    core_config.h.in
    core_config.h @ONLY)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
