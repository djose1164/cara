project(Tests)

enable_testing()

set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(LoginHelper STATIC
    loginhelper.h loginhelper.cpp)

qt_add_library(EnvHelper STATIC
    dotenv.h)

find_package(Qt6 COMPONENTS Core REQUIRED)

qt_add_resources(LoginHelper "LoginHelper_env"
    FILES .env)

target_link_libraries(LoginHelper PRIVATE Qt6::Test Qt6::Qml AuthenticationLib AuthenticationLibplugin UserLib)
target_include_directories(LoginHelper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

function(SETUP_TESTS)
       foreach(_testname ${ARGN})
           add_executable(${_testname} tst_${_testname}.cpp)
           add_test(NAME ${_testname} COMMAND ${_testname})
           target_link_libraries(${_testname} PRIVATE Qt6::Test Qt6::Qml CaraLibplugin AuthenticationLibplugin LoginHelper)
       endforeach()
endfunction()

include_directories(../Cara/Customer)
include_directories(../Cara/User)
include_directories(../Cara/Billing)
include_directories(../Cara/Authentication)
include_directories(../Cara/Authentication/business)
include_directories(../Cara)

SETUP_TESTS(
#    customerresource
#    inventoryrepository
#    localuserauthentication
#    localuserstore
   login
#    productmodel
#    productrepository
#    restuserauthentication
#    salespersoncontroller
#    salespersonrepositoryimp
#    usercredential
)

# target_compile_definitions(productrepository
#     PRIVATE CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
# )

add_subdirectory(UnitQMLTests)
add_subdirectory(Billing)
add_subdirectory(User)
