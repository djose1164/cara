enable_testing(true)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

function(SETUP_TESTS)
       foreach(_testname ${ARGN})
           qt_add_executable(${_testname} tst_${_testname}.cpp )
           add_test(NAME ${_testname} COMMAND ${_testname})
           target_link_libraries(${_testname} PRIVATE Qt6::Test Qt6::Qml ProductInventoryLib LoginHelper)
       endforeach()
endfunction()

SETUP_TESTS(
   restpricehistoryrepository
   restsupplierrepository
   sqlitesupplierrepository
   productmodel
   productrepository
)

file(COPY tesla-model-s.jpeg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
