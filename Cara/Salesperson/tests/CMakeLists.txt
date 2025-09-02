enable_testing()

function(SETUP_TESTS)
       foreach(_testname ${ARGN})
           qt_add_executable(${_testname} tst_${_testname}.cpp )
           add_test(NAME ${_testname} COMMAND ${_testname})
           target_link_libraries(${_testname} PRIVATE Qt6::Test Qt6::Qml SalespersonLib AuthenticationLib LoginHelper CommonLib)
       endforeach()
endfunction()

SETUP_TESTS(
   salespersoncontroller
   salespersonrepositoryimp
)
