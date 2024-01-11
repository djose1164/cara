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
   restcustomerrepository
   productmodel
   inventoryrepository
)

include_directories(../authentication)
include_directories(../user)
include_directories(../salesperson)
include_directories(../customer)
include_directories(../customer/data)
include_directories(../inventory)
