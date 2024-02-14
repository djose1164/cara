qt_add_library(UserLib STATIC)

set_source_files_properties(UserSettings.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(UserLib
    URI User
    VERSION 1.0
    QML_FILES
        PerfilPage.qml
        UserCredentialsForm.qml
        UserSettings.qml
    SOURCES
        address.h address.cpp
        contact.h contact.cpp
        contactrepository.h contactrepository.cpp
        data/contactrespositoryimpl.h data/contactrespositoryimpl.cpp
        data/localuserstore.h data/localuserstore.cpp
        data/userrepositoryimpl.h data/userrepositoryimpl.cpp
        service/userservice.h service/userservice.cpp
        session.h session.cpp
        usercontroller.h usercontroller.cpp
        userentity.h userentity.cpp
        usermodel.h usermodel.cpp
        userrepository.h userrepository.cpp
        usersession.h usersession.cpp
        loginmanager.h loginmanager.cpp
    DEPENDENCIES
        UtilsLib
        AuthenticationLib
        StoreLib
)

target_include_directories(UserLib
    PRIVATE {CMAKE_CURRENT_SOURCE_DIR} data service ../utils ../authentication ../store)

target_link_libraries(UserLib
    PRIVATE StoreLib)
