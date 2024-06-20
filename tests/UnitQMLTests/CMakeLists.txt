set(CMAKE_INCLUDE_CURRENT_DIR ON)
enable_testing()

add_executable(TestQML tst_login.cpp tst_login.qml
    setup.h setup.cpp
    tst_signup.qml
    Controls/tst_linearprogressbar.qml
)

add_test(NAME TestQML COMMAND TestQML)

target_link_libraries(
    TestQML
    PRIVATE Qt6::QuickTest
    PRIVATE Qt6::Quick
    PRIVATE CaraLib
    PRIVATE CaraLibplugin
    PRIVATE AuthenticationLib
    PRIVATE AuthenticationLibplugin
    PRIVATE LoginHelper
    PRIVATE DataLibplugin
    PRIVATE DataLib
)

add_definitions(-DQUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(TestQML PRIVATE ../)
