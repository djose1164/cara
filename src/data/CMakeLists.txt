qt_add_library(Data STATIC)
qt_add_qml_module(Data
    URI data
    VERSION 1.0
    SOURCES
        rest.h rest.cpp
        refresher.h refresher.cpp
        irefreshable.h irefreshable.cpp
        constants.h constants.cpp
        restconfig.h.in restconfig.h
        ibillrepository.h ibillrepository.cpp
        billrepository.h billrepository.cpp
        tokenmanager.h tokenmanager.cpp
)

set(REMOTE_API_URL https://cara-rd.live)
set(LOCAL_API_URL http://127.0.0.1:5000)

configure_file(restconfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/restconfig.h @ONLY)
