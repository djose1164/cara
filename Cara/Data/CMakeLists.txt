qt_add_library(DataLib STATIC)
qt_add_qml_module(DataLib
    URI Cara.Data
    VERSION 1.0
    SOURCES
        rest.h rest.cpp
        constants.h constants.cpp
        restconfig.h.in restconfig.h
)

target_include_directories(DataLib
    PRIVATE ../Utils ..
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

set(REMOTE_API_URL https://cara-api.onrender.com)
set(REMOTE_TESTING_API_URL https://cara-api-testing.onrender.com)
if (ANDROID)
    set(LOCAL_API_URL http://10.0.2.2:5000)
else()
    set(LOCAL_API_URL http://127.0.0.1:5000)
endif()
set(LOCAL_NETWORK_API_URL http://192.168.0.13:5000)
# RemoteTesting -- 0
# LocalTesting -- 1
# LocalNetworkTesting -- 2
# Production -- 3
set(API_ENVIRONMENT 1)

configure_file(restconfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/restconfig.h @ONLY)
