qt_add_library(AuthenticationLib STATIC)
qt_add_qml_module(AuthenticationLib
    URI Cara.Authentication
    VERSION 1.0
    QML_FILES
        LoginPage.qml
        SignUp.qml
        PasswordInput.qml
        PasswordItem.qml
    RESOURCES
        eye-crossed.svg
        eye.svg
    SOURCES
        business/authenticator.h business/authenticator.cpp
        business/irefreshable.h business/irefreshable.cpp
        business/tokenmanager.h business/tokenmanager.cpp
        business/tokennotpresent.h business/tokennotpresent.cpp
        data/refresher.h data/refresher.cpp
        data/restuserauthentication.h data/restuserauthentication.cpp
        data/tokenstore.h data/tokenstore.cpp
        entity/credential.h entity/credential.cpp
        login.h login.cpp
        service/localuserauthentication.h service/localuserauthentication.cpp
        istore.h
    DEPENDENCIES
        Utils Cara.Network
)

target_include_directories(AuthenticationLib
    PUBLIC
        business data entity service
    PRIVATE
        ../Utils ../
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(AuthenticationLib PUBLIC NetworkLib)
