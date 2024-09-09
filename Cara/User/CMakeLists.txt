qt_add_library(UserLib STATIC)

set_source_files_properties(UserSettings.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(UserLib
    URI Cara.User
    VERSION 1.0
    QML_FILES
        AddressForm.qml
        AddressItem.qml
        ContactForm.qml
        ContactInfoItem.qml
        PerfilPage.qml
        UpdatePasswordPage.qml
        UserCredentialsForm.qml
        UserSettings.qml
        CommentsDrawer.qml
    SOURCES
        address.h address.cpp
        contact.h contact.cpp
        contactrepository.h contactrepository.cpp
        data/contactrespositoryimpl.h data/contactrespositoryimpl.cpp
        data/localuserstore.h data/localuserstore.cpp
        data/userrepositoryimpl.h data/userrepositoryimpl.cpp
        loginmanager.h loginmanager.cpp
        service/userservice.h service/userservice.cpp
        session.h session.cpp
        usercontroller.h usercontroller.cpp
        userentity.h userentity.cpp
        usermodel.h usermodel.cpp
        userrepository.h userrepository.cpp
        usersession.h usersession.cpp
        comment.h comment.cpp
        commentmodel.h commentmodel.cpp
        commentrepository.h
        ordercommentrepository.h ordercommentrepository.cpp
    RESOURCES
        password.svg
    DEPENDENCIES
        Cara.Utils
        Cara.Authentication
        Cara.Store
)

target_include_directories(UserLib
    PRIVATE data service ../Utils ../
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(UserLib
    PRIVATE AuthenticationLib NetworkLib CommonLib)
