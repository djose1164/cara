enable_testing()

function(SETUP_TESTS)
       foreach(_testname ${ARGN})
           add_executable(${_testname} tst_${_testname}.cpp )
           add_test(NAME ${_testname} COMMAND ${_testname})
           target_link_libraries(${_testname} Qt6::Test CaraLib)
       endforeach()
endfunction()

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
   salespersonmanager
   salespersonrepositoryimp
   usercredential
)

qt_add_resources(restorderrepository "sample_data"
    FILES good_order.json bad_order.json
)

target_compile_definitions(productrepository
    PRIVATE CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(
    ../
    ../authentication
    ../billing
    ../customer
    ../store
    ../user
    ../utils
)

