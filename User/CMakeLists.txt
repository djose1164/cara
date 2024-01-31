qt_add_library(UserLib STATIC)

set_target_properties(UserLib PROPERTIES AUTOMOC ON)

qt_add_qml_module(UserLib
    URI User
    VERSION 1.0
    QML_FILES
        PerfilPage.qml
        UserCredentialsForm.qml
        UserSettings.qml
    SOURCES
        user.h user.cpp
        service/userservice.h service/userservice.cpp
        entity/usercredential.h entity/usercredential.cpp
        userstore.h userstore.cpp
        localuserstore.h localuserstore.cpp
        service/userserviceimpl.h service/userserviceimpl.cpp
        usersession.h usersession.cpp
        session.h session.cpp
        contact.h contact.cpp
        address.h address.cpp
        contactrepository.h contactrepository.cpp
        data/contactrespositoryimpl.h data/contactrespositoryimpl.cpp
        userentity.h userentity.cpp
        usermodel.h usermodel.cpp
        userrepository.h userrepository.cpp
        data/userrepositoryimpl.h data/userrepositoryimpl.cpp
        usercontroller.h usercontroller.cpp
)

target_include_directories(UserLib PRIVATE ../salesperson)

