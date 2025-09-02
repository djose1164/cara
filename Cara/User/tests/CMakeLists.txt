enable_testing()

function(SETUP_TESTS)
       foreach(_testname ${ARGN})
           add_executable(${_testname} tst_${_testname}.cpp )
           add_test(NAME ${_testname} COMMAND ${_testname})
           target_link_libraries(${_testname} PRIVATE Qt6::Test Qt6::Qml UserLib LoginHelper AuthenticationLib CommonLib)
       endforeach()
endfunction()

SETUP_TESTS(
   localuserauthentication
   localuserstore
   login
   restuserauthentication
   usercredential
)
