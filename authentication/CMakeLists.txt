qt_add_library(AuthenticationLib STATIC)

set_target_properties(AuthenticationLib PROPERTIES AUTOMOC ON)

qt_add_qml_module(AuthenticationLib
    URI Authentication
    VERSION 1.0
    QML_FILES
        LoginPage.qml
        SignUp.qml
    SOURCES
        login.h login.cpp
        business/authentication.h business/authentication.cpp
        data/restuserauthentication.h data/restuserauthentication.cpp
        service/localuserauthentication.h service/localuserauthentication.cpp
)

include_directories(../user)
