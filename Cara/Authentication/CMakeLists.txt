find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6Keychain REQUIRED)

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
        business/abstractaccesstokenrefresher.h
        business/tokenmanager.h business/tokenmanager.cpp
        business/tokennotpresent.h business/tokennotpresent.cpp
        data/accesstokenrefresher.h data/accesstokenrefresher.cpp
        data/restuserauthentication.h data/restuserauthentication.cpp
        entity/credential.h entity/credential.cpp
        login.h login.cpp
        service/localuserauthentication.h service/localuserauthentication.cpp
        session.h session.cpp
        abstractsession.h abstractsession.cpp
        sessionmanager.h sessionmanager.cpp
        abstractkeychain.h
        keychainimpl.h keychainimpl.cpp
    DEPENDENCIES
        Cara.Utils Cara.Network Cara.User
)

target_include_directories(AuthenticationLib
    PUBLIC
        business data entity service
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(AuthenticationLib
    PRIVATE Qt6::Core qt6keychain NetworkLib UserLib CommonLib UtilsLib)
