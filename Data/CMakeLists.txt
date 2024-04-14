qt_add_library(Data STATIC)
qt_add_qml_module(Data
    URI data
    VERSION 1.0
    SOURCES
        rest.h rest.cpp
        constants.h constants.cpp
        restconfig.h.in restconfig.h
)

target_include_directories(Data
    PRIVATE ../utils
)

set(REMOTE_API_URL https://cara-api.onrender.com)
set(LOCAL_API_URL http://127.0.0.1:5000)
set(LOCAL_NETWORK_API_URL http://192.168.0.13:5000)

configure_file(restconfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/restconfig.h @ONLY)
