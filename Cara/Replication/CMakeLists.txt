project(ReplicationLib CXX)

qt_add_library(${PROJECT_NAME} STATIC)
qt_add_qml_module(${PROJECT_NAME}
    URI Cara.Replication
    SOURCES
        bootstrapreplicationservice.h bootstrapreplicationservice.cpp
        abstractreplicationengine.h
        replicationengine.h replicationengine.cpp
        entityreplicator.h
        userreplicator.h userreplicator.cpp
        replicationmessage.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Sql)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
