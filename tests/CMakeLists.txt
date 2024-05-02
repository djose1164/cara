project(Tests VERSION 0.18.0 LANGUAGES CXX)

enable_testing()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

function(SETUP_TESTS)
       foreach(_testname ${ARGN})
           add_executable(${_testname} tst_${_testname}.cpp )
           add_test(NAME ${_testname} COMMAND ${_testname})
           target_link_libraries(${_testname} PRIVATE Qt6::Test Qt6::Qml CaraLibplugin LoginHelper)
       endforeach()
endfunction()

add_library(LoginHelper STATIC
    loginhelper.h loginhelper.cpp)

target_link_libraries(LoginHelper PRIVATE Qt6::Test Qt6::Qml CaraLibplugin)

include_directories(
    ../cara
    ../cara/store
    ../cara/authentication
    ../cara/user
    ../cara/customer
    ../cara/product
    ../cara/shopping
    ../cara/billing
)

SETUP_TESTS(
   customerresource
   inventoryrepository
   localuserauthentication
   localuserstore
   login
   pay
   payment
   productmodel
   productrepository
   restorderrepository
   restuserauthentication
   salespersoncontroller
   salespersonrepositoryimp
   usercredential
)

qt_add_resources(restorderrepository "sample_data"
    FILES good_order.json bad_order.json
)

target_compile_definitions(productrepository
    PRIVATE CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
)
