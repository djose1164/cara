qt_add_library(UtilsLib STATIC)

set_target_properties(UtilsLib PROPERTIES AUTOMOC ON)

qt_add_qml_module(UtilsLib
    URI Cara.Utils
    VERSION 1.0
    SOURCES
        fileutils.h fileutils.cpp
        httpstatuscodes.h
        notimplementated.h
        shareplataform.h shareplataform.cpp
        shareutils.h shareutils.cpp
        utils.h utils.cpp
        validationerror.h
    QML_FILES
        CaraApi.qml
        Message.qml
        SearchItem.qml
        SearchPage.qml
        SelectorHeaderItem.qml
    RESOURCES
        utils.js
)

if(ANDROID)
    target_sources(UtilsLib PRIVATE
        androidshareplataform.h androidshareplataform.cpp
    )
endif()

target_link_libraries(UtilsLib PRIVATE Qt6::Core Qt6::Network)

target_include_directories(UtilsLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
