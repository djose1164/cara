project(Tests)

enable_testing()
add_definitions(-DQUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(LoginHelper STATIC
    loginhelper.h loginhelper.cpp)

qt_add_library(EnvHelper STATIC
    dotenv.h)

target_link_libraries(LoginHelper PRIVATE Qt6::Test Qt6::Qml AuthenticationLib UserLib)
target_include_directories(LoginHelper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

function(SETUP_TESTS)
       foreach(_testname ${ARGN})
           add_executable(${_testname} tst_${_testname}.cpp )
           add_test(NAME ${_testname} COMMAND ${_testname})
           target_link_libraries(${_testname} PRIVATE Qt6::Test Qt6::Qml CaraLibplugin AuthenticationLibplugin LoginHelper)
       endforeach()
endfunction()

include_directories(../Cara/Product)
include_directories(../Cara/Customer)
include_directories(../Cara/User)
include_directories(../Cara/Billing)
include_directories(../Cara/Authentication)
include_directories(../Cara)

SETUP_TESTS(
   customerresource
   inventoryrepository
   localuserauthentication
   localuserstore
   login
   productmodel
   productrepository
   restuserauthentication
   salespersoncontroller
   salespersonrepositoryimp
   usercredential
)

target_compile_definitions(productrepository
    PRIVATE CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(UnitQMLTests)
add_subdirectory(Billing)
