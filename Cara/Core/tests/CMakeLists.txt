enable_testing()

function(SETUP_TESTS)
       foreach(_testname ${ARGN})
           add_executable(${_testname} tst_${_testname}.cpp)
           add_test(NAME ${_testname} COMMAND ${_testname})
           target_link_libraries(${_testname} PRIVATE Qt6::Test Cara::CoreLib LoginHelper)
       endforeach()
endfunction()

#set(API_BASE_URL https://cara-api.onrender.com CACHE STRING "API URL to be used when issuing requests")
#set(API_ENVIRONMENT production CACHE STRING "The type of API being used by this client")

SETUP_TESTS(
    apienvironmentservice
)

qt_add_resources(apienvironmentservice "apienvironmentservice_data"
    FILES api.env)
