project(Tests)

enable_testing()

set(CMAKE_INCLUDE_CURRENT_DIR ON)


function(SETUP_TESTS)
       foreach(_testname ${ARGN})
           add_executable(${_testname} tst_${_testname}.cpp )
           add_test(NAME ${_testname} COMMAND ${_testname})
           target_link_libraries(${_testname} PRIVATE Qt6::Test Qt6::Qml CaraLibplugin BillingLib ProductLib LoginHelper AuthenticationLib)
       endforeach()
endfunction()

# include_directories(../Cara/Product)
# include_directories(../Cara/User)

SETUP_TESTS(
   commission
   pay
   payment
   profitcalculator
   restcommissionrepository
   restorderrepository
   restsalerepository
)

qt_add_resources(restorderrepository "sample_data"
    FILES good_order.json bad_order.json
)

qt_add_resources(restsalerepository "restsalerepository_sample_data"
    FILES sales.json
)

qt_add_resources(restcommissionrepository "restcommissionrepository_sample_data"
    FILES insertable_commission.json
)
