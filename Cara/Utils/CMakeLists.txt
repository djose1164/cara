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
    QML_FILES
        BackButton.qml
        BackPage.qml
        BusyPopup.qml
        BusyStackLayout.qml
        CaraApi.qml
        Card.qml
        CenterBusyIndicator.qml
        CloseButton.qml
        CloseHeader.qml
        DateDelegate.qml
        DatePicker.qml
        DrawerHeader.qml
        Header.qml
        ListViewPage.qml
        Message.qml
        MessagePane.qml
        MoreVertButton.qml
        ProcessingIndicator.qml
        RedirectGroupBox.qml
        RedirectLabel.qml
        ScrollablePage.qml
        SearchBar.qml
        SearchButton.qml
        SearchField.qml
        SearchItem.qml
        SearchPage.qml
        Star.qml
        StarRating.qml
        TimeoutDialog.qml
        TitleHeader.qml
        PicSwipeView.qml
        EmailField.qml
        SearchHeader.qml
        SelectorHeaderItem.qml
    RESOURCES
        utils.js
        star.png
        star-unfilled.png
)

if(ANDROID)
    target_sources(UtilsLib PRIVATE
        androidshareplataform.h androidshareplataform.cpp
    )
endif()

target_link_libraries(UtilsLib PRIVATE Qt6::Core Qt6::Network)

target_include_directories(UtilsLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
