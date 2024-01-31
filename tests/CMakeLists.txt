enable_testing()

function(SETUP_TESTS)
       foreach(_testname ${ARGN})
           add_executable(${_testname} tst_${_testname}.cpp )
           add_test(NAME ${_testname} COMMAND ${_testname})
           target_link_libraries(${_testname} Qt6::Test CaraLib)
       endforeach()
endfunction()

SETUP_TESTS(
   login
   restuserauthentication
   usercredential
   salespersoncontroller
   salespersonmanager
   localuserstore
   customerresource
   productmodel
   inventoryrepository
   pay
   payment
)

include_directories(
    ../Core
    ../Models
    ../Data
    ../Entities
    ../Catalogue
    ../Inventory
    ../Salesperson
    ../User
    ../Authentication
    ../Utils
    ../Billing
    ../Customer
    ../Product
    ../Statistics
)

