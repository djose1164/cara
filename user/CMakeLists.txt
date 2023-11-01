qt_add_library(UserLib STATIC)

set_target_properties(UserLib PROPERTIES AUTOMOC ON)

qt_add_qml_module(UserLib
    URI User
    VERSION 1.0
    SOURCES
    user.h user.cpp
        entity/userentity.h entity/userentity.cpp
        service/userservice.h service/userservice.cpp
        entity/usercredentail.h entity/usercredentail.cpp
        userstore.h userstore.cpp
        localuserstore.h localuserstore.cpp
        service/userserviceimpl.h service/userserviceimpl.cpp

)

