enable_testing()

function(SETUP_TESTS)
       foreach(_testname ${ARGN})
           add_executable(${_testname} tst_${_testname}.cpp )
           add_test(NAME ${_testname} COMMAND ${_testname})
           target_link_libraries(${_testname}
              PRIVATE
                 Qt6::Test Qt6::Qml Qt6::Sql BillingLib LoginHelper AuthenticationLib
                 ProductInventoryLib CommonLib SalespersonLib CustomerLib UserLib CaraLib)
       endforeach()
endfunction()

SETUP_TESTS(
   sqlitecommissionmodel
   sqlitecommissionrepository
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

qt_add_resources(restcommissionrepository "insertable_commission"
   FILES insertable_commission.json
)

qt_add_resources(sqlitecommissionmodel "insertable_commission"
   FILES insertable_commission.json
)

qt_add_resources(commission "insertable_commission"
   FILES insertable_commission.json readable_commission.json
)
