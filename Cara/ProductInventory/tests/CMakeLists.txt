# enable_testing()
# set(CMAKE_INCLUDE_CURRENT_DIR ON)


# # include_directories(../Cara/Product)
# # include_directories(../Cara/User)
project(ProductInventoryTests LANGUAGES CXX)
find_package(Qt6 REQUIRED COMPONENTS Test)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

enable_testing(true)

function(SETUP_TESTS)
       foreach(_testname ${ARGN})
           qt_add_executable(${_testname} tst_${_testname}.cpp )
           add_test(NAME ${_testname} COMMAND ${_testname})
           target_link_libraries(${_testname} PRIVATE Qt6::Test Qt6::Qml ProductInventoryLib LoginHelper)
       endforeach()
endfunction()

SETUP_TESTS(
   restpricehistoryrepository
   productmodel
   productrepository
)
