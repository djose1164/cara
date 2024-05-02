set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_add_library(AuthenticationLib STATIC)

qt_add_qml_module(AuthenticationLib
    URI Cara.Authentication
    VERSION 1.0
    QML_FILES
        LoginPage.qml
        SignUp.qml
    SOURCES
        business/authentication.h business/authentication.cpp
        business/irefreshable.h business/irefreshable.cpp
        business/itoken.h
        business/tokenmanager.h business/tokenmanager.cpp
        business/tokennotpresent.h business/tokennotpresent.cpp
        data/refresher.h data/refresher.cpp
        data/restuserauthentication.h data/restuserauthentication.cpp
        data/tokenstore.h data/tokenstore.cpp
        entity/credential.h entity/credential.cpp
        login.h login.cpp
        service/localuserauthentication.h service/localuserauthentication.cpp
        networkrequestsingleton.h networkrequestsingleton.cpp
    DEPENDENCIES
        Utils
    IMPORTS
        Cara.Store
)

target_include_directories(AuthenticationLib
    PRIVATE
        business data entity service
        ../utils ../store
)

target_link_libraries(AuthenticationLib
    PRIVATE StoreLib)
